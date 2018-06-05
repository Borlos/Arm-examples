/*
Inicializa UARTs y usa UART0 para el puerto serie E/S
*/

int v[]={1,2,3,4,5,6,7,8,9,10}; //array de datos
int sum;

#include "string.c"     //Contiene strlen(), strcmp(), etc...
#include "uart.c"       //archivo con el codigo del driver de UART

int main(){

    int i;
    char string[64];
    UART *up;
    uart_init();    //inicializa UARTs
    up = &uart[0];  //test UART0
    uprints(up, "Introduce líneas desde el terminal 0\n\r");
    while(1){
        upgets(up, string);
        uprints(up, " ");
        uprints(up, string);
        uprints(up, "\n\r");
        if(strcmp(string, "end") == 0) break;
    }
    uprints(up, "Computa suma del array:\n\r");
    sum = 0;
    for (i = 0; i < 10; i++) sum += v[i];
    uprints(up, "sum = ");
    uputc(up, (sum/10) + '0');
    uputc(up, (sum%10) + '0');
    uprints(up, "\n\rFin del Programa\n\r");
}

