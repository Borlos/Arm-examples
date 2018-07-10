/*
Implemente un driver de UART usando los registros de datos de éste 
para carácteres E/S y comprueba los registros de flags para su lectura.
Importante incluir "defines.h" en el t.c main
*/

/**Define los offsets por bytes de los registros desde char *base **/
#define UDR         0x00
#define UDS         0x04
#define UFR         0x18
#define CNTL        0x2C
#define IMSC        0x38
#define MIS         0x40
#define SBUFSIZE    128
#define BASE        10

char *ctable = "0123456789ABCDEF";

typedef volatile struct uart{   //UART data structure
    char *base; //vase adress
    int n;      //uart number 0-3
    char inbuf[SBUFSIZE];
    int indata, inroom, inhead, intail;
    char outbuf[SBUFSIZE];
    int outdata, outroom, outhead, outtail;
    volatile int txon; // 1=TX interrupt is on
}UART;
UART uart[4]; //4 UART structures

int uart_init(){    //UART initialitation
    int i;
    UART *up;
    for(i = 0; i < 4; i++){ //uart0 to 2, adjacent
        up = &uart[i];
        up->base = (char *)(0x101F1000 + i*0x1000); //uart[i] base adress
        *(up->base+CNTL) &= ~0x10; //disable UART FIFO
        *(up->base+IMSC) |= 0x30; //Interrupt mask
        up->n = i; //ID number
        up->indata = up->inhead = up->intail = 0;
        up->inroom = SBUFSIZE;
        up->outdata = up->outhead = up->outtail = 0;
        up->outroom = SBUFSIZE;
        up->txon = 0;
    }
    uart[3].base = (char *)(0x10009000);    //uart3 en 0x10009000
}

void uart_handler(UART *up){    //Uart interrupt handler
    unsigned char mis = *(up->base + MIS); // read MIS register
    if (mis & (1<<4)) // MIS.bit4=RX interrupt
        do_rx(up);
    if (mis & (1<<5)) // MIS.bit5=TX interrupt
        do_tx(up);
}

int do_rx(UART *up){ // RX interrupt handler
    char c;
    c = *(up->base+UDR);
    kprintf("rx interrupt: %c\n", c);
    if (c==0xD)
        kprintf("\n");
    up->inbuf[up->inhead++] = c;
    up->inhead %= SBUFSIZE;
    up->indata++; up->inroom--;
}

int do_tx(UART *up){ // TX interrupt handler
    char c;
    kprintf("TX interrupt\n");
    if (up->outdata <= 0){ // if outbuf[ ] is empty
        *(up->base+IMSC) = 0x10; // disable TX interrupt
        up->txon = 0; // turn off txon flag
        return;
    }
    c = up->outbuf[up->outtail++];
    up->outtail %= SBUFSIZE;
    *(up->base+UDR) = (int)c; // write c to DR
    up->outdata--; up->outroom++;
}

int ugetc(UART *up){ // return a char from UART
    char c;
    while(up->indata <= 0); // loop until up->data > 0 READONLY
        c = up->inbuf[up->intail++];
    up->intail %= SBUFSIZE;
    // updating variables: must disable interrupts
    lock();
        up->indata--; up->inroom++;
    unlock();
    return c;
}
int uputc(UART *up, char c){ // output a char to UART
    kprintf("uputc %c ", c);
    if (up->txon){ //if TX is on, enter c into outbuf[]
        up->outbuf[up->outhead++] = c;
        up->outhead %= 128;
        lock();
            up->outdata++; up->outroom--;
        unlock();
        return;
    }
// txon==0 means TX is off => output c & enable TX interrupt
// PL011 TX is riggered only if write char, else no TX interrupt
    int i = *(up->base+UFR); // read FR
    while( *(up->base+UFR) & 0x20 ); // loop while FR=TXF
        *(up->base+UDR) = (int)c; // write c to DR
    
    //UART0_IMSC |= 0x30; // 0000 0000: bit5=TX mask bit4=RX mask
    *(up->base+IMSC) |= 0x30; // 0000 0000: bit5=TX mask bit4=RX mask
    up->txon = 1;
}

int ugets(UART *up, char *s){ // get a line from UART
    kprintf("%s", "in ugets: ");
    while ((*s = (char)ugetc(up)) != '\r'){
        uputc(up, *s++);
    }
    *s = 0;
}

int uprints(UART *up, char *s){ // print a line to UART
    while(*s)
        uputc(up, *s++);
}


//Printf Implementation
int urpu(UART *up, unsigned int x){
    char c;
    if(x){
        c = ctable[x % BASE];
        urpu(up, x / BASE);
    }
    uputc(up, c);
}

int uprintu(UART *up, unsigned int x){
    (x==0)? uputc(up, '0') : urpu(up, x);
    uputc(up, ' ');
}

int urpx(UART *up, unsigned int x){
    char c;
    if(x){
        c = ctable[x % 16];
        urpx(up, x / 16);
    }
    uputc(up, c);
}

int uprintx(UART *up, unsigned int x){
    uputc(up, '0');
    uputc(up, 'x');
    (x==0)? uputc(up, '0') : urpx(up, x);
    uputc(up, ' ');
}

int uprinti(UART *up, unsigned int x){
    if(x<0){
        uputc(up, '-');
        x = -x;
    }
    uprintu(up, x);
}

int uprintf(char *fmt, ...){
    UART *up = &uart[0];
    char *cp = fmt;
    int *ip = (int *)&fmt + 1;
    while(*cp){
        if (*cp != '%'){
            uputc(up, *cp);
            if(*cp == '\n') uputc(up, '\r');
            cp++;
            continue;
        }
        cp++;
        switch(*cp){
            case 'c': uputc(up, (char )*ip); break;
            case 's': uprints(up, (char *)*ip); break;
            case 'u': uprintu(up, (unsigned int )*ip); break;
            case 'd': uprinti(up, (int )*ip); break;
            case 'x': uprintx(up, (unsigned int )*ip); break;
        }
        cp++;
        ip++;
    }
}
