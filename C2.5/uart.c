/*
Implemente un driver de UART usando los registros de datos de éste 
para carácteres E/S y comprueba los registros de flags para su lectura.
*/

/**Define los offsets por bytes de los registros desde char *base **/
#define UDR 0x00
#define UFR 0x18
#define RXFE 0x10
#define TXFF 0x20

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

int upgetc(UART *up){ //introduce un caracter desde UART apuntado por up
    while(*(up->base+UFR) & RXFE);  //loop si UFR es REFE
    return *(up->base+UDR);         //retorna un char en UDR
}

int uputc(UART *up, char c){ //saca un caracter a UART apuntado por up
    while(*(up->base+UFR) & TXFF);  //loop si UFR es TXFF
    *(up->base+UDR) = c;            //Escribe el caracter al registro de datos
}

int upgets(UART *up, char *s){  //introduce string de carácteres
    while((*s = upgetc(up)) != '\r'){
        uputc(up, *s);
        s++;
    }
    *s = 0;
}

int uprints(UART *up, char *s){ //Saca string de carácteres
    while(*s) uputc(up, *s++);
}

 
