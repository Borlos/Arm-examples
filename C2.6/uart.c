/*
Implemente un driver de UART usando los registros de datos de éste 
para carácteres E/S y comprueba los registros de flags para su lectura.
*/

/**Define los offsets por bytes de los registros desde char *base **/
#define UDR 0x00
#define UFR 0x18
#define BASE 10

char *ctable = "0123456789ABCDEF";

typedef volatile struct uart{   //define la estructura de datos de un UART
    char *base; //Direccion base como char*
    int n;      //uart numero 0-3
}UART;
UART uart[4]; //4 estructuras UART

int uart_init(){    //función de inicialización de UART
    int i;
    UART *up;
    for(i = 0; i < 3; i++){ //uart0 a uart2, adyacentes
        up = &uart[i];
        up->base = (char *)(0x101F1000 + i*0x1000);
        up->n = i;
    }
    uart[3].base = (char *)(0x10009000);    //uart3 en 0x10009000
}

int ugetc(UART *up){ //introduce un caracter desde UART apuntado por up
    while(*(up->base+UFR) & RXFE);  //loop si UFR es REFE
    return *(up->base+UDR);         //retorna un char en UDR
}

int uputc(UART *up, char c){ //saca un caracter a UART apuntado por up
    while(*(up->base+UFR) & TXFF);  //loop si UFR es TXFF
    *(up->base+UDR) = c;            //Escribe el caracter al registro de datos
}

int ugets(UART *up, char *s){  //introduce string de carácteres
    while((*s = ugetc(up)) != '\r'){
        uputc(up, *s);
        s++;
    }
    *s = 0;
}

int uprints(UART *up, char *s){ //Saca string de carácteres
    while(*s) uputc(up, *s++);
}

//Implementación de uprintf

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
