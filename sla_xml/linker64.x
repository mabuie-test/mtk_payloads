OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)

ENTRY(cmd_security_set_remote_sec_policy)

SECTIONS
{
  . = 0x40000000;
  .text : { *(.text.start) *(.text*) }
  .rodata : { *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) . = ALIGN(8); }
  /DISCARD/ : { *(.comment*) *(.note*) *(.eh_frame*) *(.ARM*) *(.plt*) *(.got*) }
}
