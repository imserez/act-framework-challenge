OUTPUT_ARCH( "riscv" )
ENTRY ( main )
SECTIONS
{
    . = 0x80000000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }

    . = ALIGN(8);
}