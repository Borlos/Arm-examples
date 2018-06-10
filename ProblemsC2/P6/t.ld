ENTRY(reset_start)
SECTIONS
{
    . = 0x10000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
    .data : { *(i1.o, i2.o, i3.o, i4.o, i5.o, i6.o, i7.o, i8.o, i9.o, i10.o, i11.o, i12.o, i13.o, i14.o, i15.o) } 
    . =ALIGN(8);
    . =. + 0x1000;
    stack_top =.;
}
