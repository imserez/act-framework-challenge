OUTPUT_ARCH( "riscv" )
ENTRY( _start )
SECTIONS
{
  . = 0x80000000;
  .text : { *(.text) }
  .data : { *(.data) }
  .bss  : { *(.bss) }

  . = ALIGN(8);
  PROVIDE(_stack_top = . + 0x1000); /* Damos 4KB de Stack */
}