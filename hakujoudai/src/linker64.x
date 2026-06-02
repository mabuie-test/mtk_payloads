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


    .bss : {
        __bss_start = .;
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    }
  .pointers_table ALIGN(4) : {
    __pointers_start = .;
    KEEP(*(.pointer_table))
    __pointers_end = .;
  }
  /DISCARD/ : { *(.comment*) *(.note*) *(.eh_frame*) *(.ARM*) *(.plt*) *(.rela*) *(.dynsym*) *(.dynstr*) *(.dynamic*) }
}
