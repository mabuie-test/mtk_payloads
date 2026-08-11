OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(cmd_boot_to)

SECTIONS
{
  . = 0x40000000;
  .text     : { *(.text.start)  *(.text.main) *(.text   .text.*   .gnu.linkonce.t.*) }
  .rodata   : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
  .data     : { *(.data   .data.*   .gnu.linkonce.d.*) }
  .bss      : ALIGN(4) {
    __bss_start = .;
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
  }
  /DISCARD/ : { *(.interp) *(.dynsym) *(.dynstr) *(.hash) *(.dynamic) *(.comment) }
}
