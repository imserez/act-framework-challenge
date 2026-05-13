OUTPUT_ARCH( "riscv" )
ENTRY ( main )
SECTIONS
{
    . = 0x80010000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }

    . = ALIGN(8);
}