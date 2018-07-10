#ifndef __DEFINES_H__
#define __DEFINES_H__

//Uart Defines
#define TXFE 0x80
#define RXFF 0x40
#define TXFF 0x20
#define RXFE 0x10
#define BUSY 0x08

//Timer defines
#define TLOAD 0x0
#define TVALUE 0x1
#define TCNTL 0x2
#define TINTCLR 0x3
#define TRIS 0x4
#define TMIS 0x5
#define TBGLOAD 0x6

//Vid Defines
#define RED 0
#define BLUE 1
#define GREEN 2
#define WHITE 3
#define OFFSET 1626 //font file starts chars at bit 1627

//Vectored Int (VIC) addresses
#define VicBase 0x10140000
#define VIC_STATUS              (*((volatile unsigned int *)(VicBase+0x00)))
#define VIC_FIQ_STATUS          (*((volatile unsigned int *)(VicBase+0x04)))
#define VIC_RAW_STATUS          (*((volatile unsigned int *)(VicBase+0x08)))
#define VIC_INT_SELECT          (*((volatile unsigned int *)(VicBase+0x0c)))    /* 1 = FIQ, 0 = IRQ */
#define VIC_INTENABLE           (*((volatile unsigned int *)(VicBase+0x10)))    /* 1 = enable, 0 = disable */
#define VIC_INT_ENABLE_CLEAR    (*((volatile unsigned int *)(VicBase+0x14)))
#define VIC_INT_SOFT            (*((volatile unsigned int *)(VicBase+0x18)))
#define VIC_INT_SOFT_CLEAR      (*((volatile unsigned int *)(VicBase+0x1c)))
#define VIC_PROTECT             (*((volatile unsigned int *)(VicBase+0x20)))
#define VIC_PL190_VECT_ADDR     (*((volatile unsigned int *)(VicBase+0x30)))   /* PL190 only */
#define VIC_PL190_DEF_VECT_ADDR (*((volatile unsigned int *)(VicBase+0x34)))    /* PL190 only */

#define VIC_VECT_ADDR0          (*((volatile unsigned int *)(VicBase+0x100)))   /* 0 to 15 (0..31 PL192) */
#define VIC_VECT_CNTL0          (*((volatile unsigned int *)(VicBase+0x200)))   /* 0 to 15 (0..31 PL192) */
#define VIC_ITCR                (*((volatile unsigned int *)(VicBase+0x300)))   /* VIC test control register */

//Secondary Int (SIC) adresses
#define SicBase 0x10003000

#define SIC_STATUS              (*((volatile unsigned int *)(SicBase+0x00)))
#define SIC_INTENABLE           (*((volatile unsigned int *)(SicBase+0x08)))    /* 1 = enable, 0 = disable */

#endif
