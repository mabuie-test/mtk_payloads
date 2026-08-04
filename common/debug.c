/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Shomy
 */

#include "include/nanoprintf.h"
#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h>
#include <drivers/uart.h>
#include <libc.h>

#define NANOPRINTF_IMPLEMENTATION

#include <debug.h>
#include <mmio.h>

static printf_putc_cb cb_table[PRINTF_MAX_CALLBACKS];
static int            cb_count = 0;

int printf_register_cb(printf_putc_cb cb)
{
    if (cb == NULL)
        return 0;

    if (cb_count >= PRINTF_MAX_CALLBACKS)
        return -1;

    cb_table[cb_count++] = cb;
    return 0;
}

int printf_unregister_cb(printf_putc_cb cb)
{
    for (int i = 0; i < cb_count; i++) {
        if (cb_table[i] == cb) {
            // Shift remaining entries down to fill the gap
            for (int j = i; j < cb_count - 1; j++)
                cb_table[j] = cb_table[j + 1];

            cb_table[--cb_count] = NULL;
            return 0;
        }
    }
    return -1;
}

void printf_unregister_all_cbs(void)
{
    for (int i = 0; i < cb_count; i++)
        cb_table[i] = NULL;
    cb_count = 0;
}

static void dispatch_putc(int ch, void *ctx)
{
    (void)ctx;
    for (int i = 0; i < cb_count; i++)
        cb_table[i](ch, NULL);
}

int printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int ret = npf_vpprintf(&dispatch_putc, NULL, fmt, args);
    va_end(args);
    return ret;
}

void hexdump(const void *data, size_t size, uint64_t base_addr)
{
    const uint8_t *bytes = (const uint8_t *)data;

    for (size_t i = 0; i < size; i += 16) {
#ifdef __aarch64__
        printf("%016" PRIx64 ": ", base_addr + i);
#else
        printf("%08" PRIx32 ": ", (uint32_t)(base_addr + i));
#endif
        for (size_t j = 0; j < 16; j++) {
            if (i + j < size)
                printf("%02x ", bytes[i + j]);
            else
                printf("   ");
        }

        printf(" |");
        for (size_t j = 0; j < 16 && i + j < size; j++) {
            uint8_t c = bytes[i + j];
            printf("%c", (c >= 32 && c < 127) ? c : '.');
        }
        printf("|\n");
    }
}

static inline int isdigit(char c) {
    return (c >= '0' && c <= '9');
}

static inline int isxdigit(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

static inline int hexval(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return c - 'A' + 10;
}


void bytes_to_hex(const uint8_t *in, size_t len, char *out) {
    for (size_t i = 0; i < len; i++)
        npf_snprintf(out + i * 2, 3, "%02x", in[i]);
}

int hex_to_bytes(const char *hex, uint8_t *out, size_t max_out_len) {
    if (!hex || !out) return -1;

    size_t hex_len = strlen(hex);

    if (hex_len % 2 != 0) return -1;

    size_t byte_len = hex_len / 2;

    if (byte_len > max_out_len) return -1;

    for (size_t i = 0; i < byte_len; i++) {
        int hi = hexval(hex[i * 2]);
        int lo = hexval(hex[i * 2 + 1]);

        if (hi < 0 || lo < 0) return -1;

        out[i] = (uint8_t)((hi << 4) | lo);
    }

    return (int)byte_len;
}
