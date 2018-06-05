#include "defines.h"
#include "vid.c"
#include "uart.c"

extern char _binary_image1_start, _binary_image2_start;

#define WIDTH 640

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
    uprintf("\nImagen BMP, altura=%d  anchura=%d\n", h, w);
}

int main(){
    char c, *p;
    uart_init();    //inicializa UARTs
    UART *up = &uart[0];    //usa UART0
    fbuf_init(1);    //Modo VGA por defecto
    while(1){
        p = &_binary_image1_start;
        show_bmp(p, 0, 80);     //muestra imagen 1
        uprintf("Introduce una tecla desde esta UART: ");
        ugetc(up);
        p = &_binary_image2_start;
        show_bmp(p, 120, 0);    //muestra imagen 2
    }
    while(1);   //entra en bucle infinito
}
