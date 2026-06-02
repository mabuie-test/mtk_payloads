OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

ENTRY(main)

SECTIONS
{
  . = 0x68000000;

  .text : {
    *(.text.main)
    *(.text*)
  }

  .rodata : {
    *(.rodata*)
  }

  .data : {
    *(.data*)
  }

  .pointers_table ALIGN(4) : {
    __pointers_start = .;
    KEEP(*(.pointer_table))
    __pointers_end = .;
  }

  .bss ALIGN(32) : {
      __bss_start = .;
      *(.bss*)
      *(COMMON)
      __bss_end = .;
  }

  /DISCARD/ : {
    *(.comment*) *(.note*) *(.eh_frame*) *(.ARM*) *(.plt*) *(.got*)
  }
}
