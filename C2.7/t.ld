ENTRY(reset_start)
SECTIONS
{
    . = 0x10000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
    .data : { *(image.o) } 
    .data : { *(font.o) }
    . =ALIGN(8);
    . =. + 0x1000;
    stack_top =.;
}
