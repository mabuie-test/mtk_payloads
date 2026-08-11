OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

ENTRY(cmd_boot_to)

SECTIONS
{
  . = 0x40000000;
  .text : { *(.text.start) *(.text*) }
  .rodata : { *(.rodata*) }
  .data : { *(.data*) }
  .bss      : ALIGN(4) {
    __bss_start = .;
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
  }
  /DISCARD/ : { *(.comment*) *(.note*) *(.eh_frame*) *(.ARM*) *(.plt*) *(.got*) }
}
