#include "defines.h" //LCD, TIMER and UART adresses
#include "string.c"  //strcmp, strlen, etc
#include "vid.c"   //timer handler file
#include "timer.c"     //LCD driver file
#include "exceptions.c"    //other exceptions handlers

void copy_vectors(void){    //copy vector table in ts.s to 0x0
    extern unsigned int vectors_start, vectors_end;
    unsigned int *vectors_src = &vectors_start;
    unsigned int *vectors_dst = (unsigned int *)0;
    while (vectors_src < &vectors_end) *vectors_dst++ = *vectors_src++;
}

//void timer_handler();
TIMER *tp[4];   //4 TImer structure pointers

void IRQ_handler(){     //IRQ interrupt handler in C
    int vicstatus = VIC_STATUS;     //Read VIC status registers to determine interrupt source
    if(vicstatus & (1<<4)){         //VIC status BITs: timer0, 1=4, uart0=13, uart1=14
        if(*(tp[0]->base+TVALUE)==0) timer_handler(0);  //timer 0
        if(*(tp[1]->base+TVALUE)==0) timer_handler(1);  //timer 1
    }
    if (vicstatus & (1<<5)){        //bit5 = 1: timer2,3
        if(*(tp[2]->base+TVALUE)==0) timer_handler(2);
        if(*(tp[3]->base+TVALUE)==0) timer_handler(3);
    }
}    

int main(){
    int i;  
    color = RED;        //Color in vid.c
    fbuf_init();        //initialize LCD driver
    kprintf("main starts\n");
    VIC_INTENABLE = 0;  //Enable VIC for timer interrupts
    VIC_INTENABLE |= (1<<4);    //timer0,1 at VIC.bit4
    VIC_INTENABLE |= (1<<5);    //timer2,3 at VIC.bit5
    timer_init();
    for (i=0; i<4; i++){    //start all 4 timers
        tp[i] = &timer[i];
        timer_start(i);
    }
    kprintf("Enter while(1) loop, handle timer interrupts\n");
    while(1);
}
