#include "defines.h"
#include "vid.c"
#include "uart.c"

extern char _binary_i1_start, _binary_i2_start, _binary_i3_start, _binary_i4_start, 
            _binary_i5_start, _binary_i6_start, _binary_i7_start, _binary_i8_start, 
            _binary_i9_start, _binary_i10_start, _binary_i11_start, _binary_i12_start, 
            _binary_i13_start, _binary_i14_start, _binary_i15_start;

#define WIDTH 640

void delay(int time){
    int i = 0;
    while(i < time) i++;
}
    

int show_bmp(char *p, int start_row, int start_col){
    int h, w, pixel, rsize, i, j;
    unsigned char r, g, b;
    char *pp;
    int *q = (int *)(p+14);     //Salta la cabecera de 14 bytes
    w = *(q+1);                 //W, anchura de imagen en pixeles
    h = *(q+2);                 //H altura de imagen en pixeles
    p += 54;                    //p -> pixeles de la imagen
    //las imagenes BMP estan boca abajo, cada linea es multuplo de 4 bytes
    rsize = 4*((3*w + 3)/4);    //multiplo de 4
    p += (h-1)*rsize;           //ultima linea de pixeles
    for (i = start_row; i < start_row + h; i++){
        pp = p;
        for (j = start_col; j < start_col + w; j++){
            b = *pp; g = *(pp+1); r = *(pp+2);  //Valores RGB
            pixel = (b<<16) | (g<<8) | r;       //valor del pixel
            fb[i*WIDTH + j] = pixel;            //Escribe al frame buffer
            pp += 3;
        }
        p -= rsize;     //linea anterior
    }
}

int main(){
    char c;
    int i;
    char *p[] ={&_binary_i1_start, &_binary_i2_start, &_binary_i3_start, &_binary_i4_start, 
                &_binary_i5_start, &_binary_i6_start, &_binary_i7_start, &_binary_i8_start, 
                &_binary_i9_start, &_binary_i10_start, &_binary_i11_start, &_binary_i12_start, 
                &_binary_i13_start, &_binary_i14_start, &_binary_i15_start};
    uart_init();    //inicializa UARTs
    UART *up = &uart[0];    //usa UART0
    fbuf_init();    //Modo VGA por defecto
    while(1){
        for(i = 0; i < 15; i++){
        show_bmp(p[i], 0, 0);
        delay(5000000);
        }
    }
    while(1);   //entra en bucle infinito
}
