ENTRY(reset_start)
SECTIONS
{
    . = 0x10000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
    .data : { *(image1.o) } 
    .data : { *(image2.o) }
    . =ALIGN(8);
    . =. + 0x1000;
    stack_top =.;
}
