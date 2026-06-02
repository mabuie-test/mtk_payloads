OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(main)

SECTIONS
{
  . = 0x68000000;
  .text     : { *(.text.main) *(.text   .text.*   .gnu.linkonce.t.*) }
  .rodata   : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
  .data     : { *(.data   .data.*   .gnu.linkonce.d.*) }
  .pointers_table ALIGN(4) : {
    __pointers_start = .;
    KEEP(*(.pointer_table))
    __pointers_end = .;
  }
  .bss      : {
    __bss_start = .;
    *(.bss    .bss.*    .gnu.linkonce.b.*) *(COMMON)
    __bss_end = .;
  }
  /DISCARD/ : { *(.interp) *(.dynsym) *(.dynstr) *(.hash) *(.dynamic) *(.comment) }
}
