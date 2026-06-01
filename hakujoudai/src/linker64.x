OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

ENTRY(main)

SECTIONS
{
  . = 0;
  .text : { *(.text.start) *(.text*) }
  .rodata : { *(.rodata*) }
  .data : { *(.data*) }
  .got : { *(.got*) }
  .got.plt : { *(.got.plt*) }

  .pointers_table ALIGN(4) : {
    __pointers_start = .;
    KEEP(*(.pointer_table))
    __pointers_end = .;
  }

  .bss : { *(.bss*) *(COMMON) . = ALIGN(8); }
  /DISCARD/ : { *(.comment*) *(.note*) *(.eh_frame*) *(.ARM*) *(.plt*) *(.rela*) *(.dynsym*) *(.dynstr*) *(.dynamic*) }
}
