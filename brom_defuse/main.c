/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * SPDX-FileCopyrightText: (c) 2025-2026 Shomy
 *
 * Based on the work of bkerler and k4y0z (c) 2021, GPLv3
 */

#include <stdint.h>
#include <stddef.h>

#define UART_BASE_DEFAULT 0x11002000
// Constants taken from kaeru:
// https://github.com/R0rt1z2/kaeru/blob/main/drivers/uart/mtk_uart.c
#define UART_THR_OFFSET 0x00
#define UART_LSR_OFFSET 0x14
#define UART_LSR_THRE   0x20

// Sent before and after patching
#define ACK 0x00000000

/*
 * LOG FORMAT for debugging:
 *
 * O:XXX   -> Found XXX
 * F:XXX   -> Failed to find XXX
 * B:XXX   -> Before some action XXX
 * A:XXX   -> After some action XXX
 *
 */
#if DEBUG
#define LOG(x) do { print(x); print("\n"); } while(0)
#else
#define LOG(x)
#endif

typedef struct {
    uint32_t brom_base;
    uint32_t brom_start;
    uint32_t brom_end;

    /* UART */
    volatile uint32_t* uart_lsr;
    volatile uint32_t* uart_thr;

    /* Security */
    // Holds the buffer with
    volatile uint32_t** sec_reg1;
    volatile uint32_t** sec_reg2;
    uint32_t sec_offset;
    uint32_t fusebuffer[0x10]; // Used to replace the original data

    /* Handlers */
    uint32_t (*usbdl_put_data)(void* buf, uint32_t size);
    uint32_t (*usbdl_get_data)(void* buf, uint32_t size);
    uint32_t (*usbdl_put_word)(uint32_t word);
    uint32_t (*cmd_handler)();
} ctx_t;

static volatile ctx_t g_ctx;


static inline void mtk_uart_putc(char ch)
{
    while (!((*g_ctx.uart_lsr) & UART_LSR_THRE));
    *g_ctx.uart_thr = ch;
}

static inline void uart_putc(char ch)
{
    if (ch == '\n')
        mtk_uart_putc('\r');
    mtk_uart_putc(ch);
}

int print(const char* s)
{
    int count = 0;
    while (*s) {
        uart_putc(*s++);
        count++;
    }
    return count;
}


/*
 * Parse LDR (literal) Thumb instruction and calculate literal address
 * Format: LDR Rn, =<literal>
 * @pc: Current program counter
 * @instr: Thumb instruction (16-bit)
 * @rt: Pointer to target register output
 * Returns: Pointer to literal value
 */
static uint32_t *find_ldr_literal(uint32_t pc, uint16_t instr, uint8_t *rn)
{
    /*
     * LDR (literal) encoding:
     * 01001 000 00000000 (0x4800)
     * First 5 bits: LDR opcode
     * Next 3 bits: destination register (Rn)
     * Last 8 bits: immediate value (imm8), which gives offset from PC in words
     * More info: https://github.com/shomykohai/penumbra/blob/main/scripts/find_crypto_offsets.py#L67
     */
    uint8_t imm8 = instr & 0xFF;
    if (rn)
        *rn = (instr >> 8) & 0x7;

    // Align PC to 4-byte
    pc = (pc & ~3);

    // Literal pool address = aligned PC + (imm8 * 4) + 4 (PC is 4 bytes ahead)
    return (uint32_t *)(pc + (imm8 << 2) + 4);
}

/*
 * Search for a pattern in memory with optional mask support.
 * If mask is NULL, all bits must match exactly.
 * If mask is provided, only bits set in mask are compared (e.g., 0xFF00 matches high byte only).
 * @start: Start address
 * @end: End address
 * @pattern: Pattern to search for
 * @plen: Length of the pattern (in uint16_t)
 * @mask: Optional mask (NULL = match all bits, 0xFFFF = match all, 0xFF00 = match high byte only, etc.)
 * Returns: Address where the pattern was found, or 0 if not found
 */
 static uint32_t search_pattern(uint32_t start, uint32_t end, const uint16_t* pattern, uint32_t plen, const uint16_t* mask)
 {
     for (uint32_t addr = start; addr < end; addr += 2) {
         uint16_t *curr = (uint16_t *)addr;
         uint32_t i = 0;

         while (i < plen) {
             uint16_t val = curr[i];
             uint16_t pat = pattern[i];

             if (mask) {
                 if ((val & mask[i]) != (pat & mask[i]))
                     break;
             } else {
                 if (val != pat)
                     break;
             }
             i++;
         }
         if (i == plen)
             return addr;
     }
     return 0;
 }

/*
 * Find the UART base address, and set the global context accordingly.
 * @brom_base: Base address of the BootROM
 * Returns: void
 */
static void find_uart(void)
{
    // DAT_0000CF8 - 0x8, 0x4e455f31 constant
    static const uint16_t uart_pattern[] = {0x5F31, 0x4E45, 0x0F93};
    uint32_t addr = search_pattern(g_ctx.brom_base + 0x100, g_ctx.brom_base + 0x14000, uart_pattern, 3, NULL);

    if (addr) {
        // The pattern finds an instruction from which *then* we can get the base address
        volatile uint32_t *base = (volatile uint32_t*)(((uint32_t*)(addr + 0x8))[0] & 0xFFFFFFFF);
        g_ctx.uart_lsr = (volatile uint32_t *)((uintptr_t)base + UART_LSR_OFFSET);
        g_ctx.uart_thr = (volatile uint32_t *)((uintptr_t)base + UART_THR_OFFSET);
        LOG("O:UART");
        return;
    }

    /* We shouldn't be here!!! */
    LOG("F:UART");
}


/*
 * Find USB data transfer functions in BootROM
 * @brom_start: Start address of BootROM
 * @brom_end: End address of BootROM
 * Returns: void
 */
static unsigned char find_usb(uint32_t brom_start, uint32_t brom_end)
{
    static const uint16_t usb_pattern[] = {0x0AA0, 0x0550, 0x0000};
    static const uint16_t usb_mask[] = {0xFFFF, 0xFFFF, 0xFF00};

    LOG("B:USB");
    uint32_t addr = search_pattern(brom_start, brom_end, usb_pattern, 3, usb_mask);
    if (!addr)
        goto fail_usb;

    g_ctx.usbdl_put_word = (void*)(*((uint32_t*)(addr - 0x1C)));
    g_ctx.usbdl_get_data = (void*)(*((uint32_t*)(addr - 0x10)) | 1);
    g_ctx.usbdl_put_data = (void*)(*((uint32_t*)(addr - 0x0C)) | 1);

    // For jumping to the payload, we overwritten this pointer with the address of the payload.
    // To avoid issues, we need to fix the pointer, so that we can still use usbdl_* functions.
    uint32_t *ptr_table = find_ldr_literal((uint32_t)g_ctx.usbdl_put_word + 7,
                                          ((uint16_t*)(g_ctx.usbdl_put_word + 7))[0], NULL);
    *(volatile uint32_t*)(ptr_table[0] + 8) = (uint32_t)ptr_table[2];

    LOG("O:USB");
    return 0;

fail_usb:
    LOG("F:USB");
    return 1;
}

/*
 * Finds the registers where the fuse cache is stored
 * @brom_start: Start address of BootROM
 * @brom_end: End address of BootROM
 * Returns: void
 */
static void find_sec_regs(uint32_t brom_start, uint32_t brom_end)
{
    g_ctx.sec_offset = 0;
    g_ctx.sec_reg1 = NULL;
    g_ctx.sec_reg2 = NULL;

    static const uint16_t sbc_pattern1[] = {0x4800, 0x68c1};
    static const uint16_t sbc_mask1[] = {0xFF00, 0xFFFF};
    uint32_t addr = search_pattern(brom_start, brom_end, sbc_pattern1, 2, sbc_mask1);
    if (addr) {
        LOG("B:SEC1");
        // Sec Mode 1 (Newer devices)

        // The LDR loads the literal address, which contains the address of the security register
        uint32_t *literal_ptr = find_ldr_literal(addr, ((uint16_t*)addr)[0], NULL);
        uint32_t base_addr = *literal_ptr;

        // Sec Reg1 is loaded from [base_addr + 0xC]
        g_ctx.sec_reg1 = (volatile uint32_t **)(base_addr + 0xC);

        // SEC_REG2: next DAT in the literal pool (just +4 bytes)
        // This seem to control whether ignoring security features is possible:
        // else if (_DAT_001027a4 == 0)
        uint32_t base_addr2 = *(literal_ptr + 1);  // Next uint32_t in pool
        g_ctx.sec_reg2 = (volatile uint32_t **)(base_addr2 + 0x4);
        g_ctx.sec_offset = 0x28;
        LOG("O:SEC1");
        return;
    }

    // LDR R1, =DAT_00007B98, MT6572
    static const uint16_t sbc_pattern0[] = {0x4900, 0x6800, 0x2900, 0xD003, 0x6C09};
    static const uint16_t sbc_mask0[] = {0xFF00, 0xFF00, 0xFFFF, 0xFFFF, 0xFFFF};
    addr = search_pattern(brom_start, brom_end, sbc_pattern0, 5, sbc_mask0);
    if (addr) {
        LOG("B:SEC0");
        // Sec Mode 0 (Older devices)
        uint32_t *literal_ptr = find_ldr_literal(addr, ((uint16_t*)addr)[0], NULL);
        uint32_t base_addr = *literal_ptr;
        g_ctx.sec_reg1 = (volatile uint32_t **)(base_addr);
        g_ctx.sec_offset = 0x40;
        LOG("O:SEC0");
        return;
    }

    LOG("F:SEC");
}

/*
 * Find the command handler function in BootROM
 * Sets g_ctx.cmd_handler if found
 * Returns: void
 */
static void find_cmdhandler(void) {
    static const uint16_t cmd_pattern[] = {0xF7FF, 0xFF00, 0xF7FF, 0x0000, 0x4604};
    static const uint16_t cmd_mask[] = {0xFFFF, 0xFF00, 0xFFFF, 0x0000, 0xFFFF};

    LOG("B:CMDH");
    uint32_t addr = search_pattern(g_ctx.brom_start, g_ctx.brom_end, cmd_pattern, 5, cmd_mask);
    if (addr) {
        g_ctx.cmd_handler = (void*)(addr | 1);
        LOG("O:CMDH");
        return;
    }
    LOG("F:CMDH");
}

__attribute__ ((section(".text.main"))) int main(){
    uint32_t ack = ACK;

    /* Setup the global context  */
    g_ctx.brom_base = 0x0;
    // LDR PC, [PC, #-4]
    volatile uint32_t *brom_ptr = (volatile uint32_t *)(g_ctx.brom_base);
    if (brom_ptr[0] == 0xe51ff004) {
        g_ctx.brom_base = brom_ptr[1];
    }


    g_ctx.brom_start = g_ctx.brom_base + 0x100;
    g_ctx.brom_end = g_ctx.brom_base + 0x20000;
    g_ctx.uart_lsr = (volatile uint32_t *)(UART_BASE_DEFAULT + UART_LSR_OFFSET);
    g_ctx.uart_thr = (volatile uint32_t *)(UART_BASE_DEFAULT + UART_THR_OFFSET);
    for (int i = 0; i < 0x10; i++) {
        g_ctx.fusebuffer[i] = 0;
    }

    /* Time to search! */
    find_uart();

    print("*** Generic brom patcher ***\n");
    print("(c) bkerler, k4y0z 2021, shomy 2025\n");

    if (find_usb(g_ctx.brom_start, g_ctx.brom_end) != 0) {
        return 0;
    }

    LOG("B:ACK");
    g_ctx.usbdl_put_word(ack);
    LOG("A:ACK");

    find_sec_regs(g_ctx.brom_start, g_ctx.brom_end);


    /* Now we patch! */
    g_ctx.fusebuffer[0] = 0x1; // Skip this error => else if (_DAT_001027a4 == 0), as well as SLA check
    if (g_ctx.sec_offset == 0x40) {
        // Mode 0
        LOG("B:PATCH0");
        *g_ctx.sec_reg1 = (volatile uint32_t *)&g_ctx.fusebuffer;
        g_ctx.fusebuffer[g_ctx.sec_offset / 4] = 0xB; // 0xB = b1011
    }
    else if (g_ctx.sec_offset == 0x28) {
        // Mode 1
        LOG("B:PATCH1");
        *((volatile uint32_t *)(g_ctx.sec_reg1)) = 0x1;
        *((volatile uint32_t *)(g_ctx.sec_reg1 + 2)) = 0xB;
        *g_ctx.sec_reg2 = (volatile uint32_t *)&g_ctx.fusebuffer;
        g_ctx.fusebuffer[g_ctx.sec_offset / 4] = 0x700; // 0x700 = b0111 0000 0000, << 0x1e (DAA), << 0x1c (SBC), +0x28 & 1 (SLA)
    }

    LOG("A:PATCH");

    find_cmdhandler();

    /* Finally, invalidate the cache */
    LOG("B:ICACHE");
    asm volatile ("mcr p15, 0, %0, c7, c5, 0" : : "r" (0));
    LOG("A:ICACHE");

    return g_ctx.cmd_handler();
}
