#include "defines.h" //LCD, TIMER and UART adresses
#include "string.c"  //strcmp, strlen, etc
#include "vid.c"   //timer handler file
#include "uart.c"
#include "kbd.c"
#include "timer.c"     //LCD driver file
#include "exceptions.c"    //other exceptions handlers

void copy_vectors(void){    //copy vector table in ts.s to 0x0
    extern unsigned int vectors_start, vectors_end;
    unsigned int *vectors_src = &vectors_start;
    unsigned int *vectors_dst = (unsigned int *)0;
    while (vectors_src < &vectors_end) *vectors_dst++ = *vectors_src++;
}

//void timer_handler();
//TIMER *tp[4];   //4 TImer structure pointers

void IRQ_handler(){     //IRQ interrupt handler in C
    int vicstatus = VIC_STATUS;     //Read VIC status registers to determine interrupt source
    int sicstatus = SIC_STATUS;     //Read SIC status registers
    if(vicstatus & (1<<4)){         //VIC status BITs: timer0
        timer_handler(0);           //timer 0
    }
    if (vicstatus & (1<<12)) // bit12=1: uart0
        uart_handler(&uart[0]);
    if (vicstatus & (1<<13)) // bit13=1: uart1
        uart_handler(&uart[1]);
    if (vicstatus & (1<<31)){       //PIC.bit31 = SIC interrupts
        if(sicstatus & (1<<3)){
            kbd_handler();
        }
    }
}    

int main(){
    char line[128];
    UART *up;
    KBD *kp;
    fbuf_init();        //initialize LCD driver
    kprintf("C3.3 start, test KBD, Timer, Uart drivers\n");
    VIC_INTENABLE = 0;          //Enable VIC for timer interrupts
    VIC_INTENABLE |= (1<<4);    //timer0,1 at VIC.bit4
    VIC_INTENABLE |= (1<<5);    //timer2,3 at VIC.bit5
    VIC_INTENABLE |= (1<<12);    //timer0,1 at VIC.bit4
    VIC_INTENABLE |= (1<<13);    //timer2,3 at VIC.bit5
    VIC_INTENABLE |= (1<<31);   //SIC to PIC.bit31
    SIC_INTENABLE = 0;          //For Secondary Int, KBD
    SIC_INTENABLE |= (1<<3);    //KBD int = SIC.bit3
    kbd_init();
    uart_init();
    up = &uart[0];
    kp = &kbd;
    timer_init();
    timer_start(0);             //Only starts timer 0
    while(1){
        kprintf("Enter a line from KBD\n");
        kgets(line);
        kprintf("Enter a line from UART0\n");
        uprints(up, "Enter a line from UARTS\n\r");
        ugets(up, line);
        uprintf("%s\n", line);
    }
}
