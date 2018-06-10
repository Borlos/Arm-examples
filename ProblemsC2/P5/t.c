#include "defines.h"
#include "vid.c"
#include "uart.c"

extern char _binary_image_start;

#define WIDTH 640

int show_bmp(char *p, int start_row, int start_col, int mode){
    int h, w, d=1, pixel, rsize, i, j;
    unsigned char r, g, b;
    char *pp;
    int *q = (int *)(p+14);     //Salta la cabecera de 14 bytes
    w = *(q+1);                 //W, anchura de imagen en pixeles
    h = *(q+2);                 //H altura de imagen en pixeles
    p += 54;                    //p -> pixeles de la imagen
    //las imagenes BMP estan boca abajo, cada linea es multuplo de 4 bytes
    rsize = 4*((3*w + 3)/4);    //multiplo de 4
    p += (h-1)*rsize;           //ultima linea de pixeles
    uprintf("\nOriginal Size: width=%d heigh=%d",w,h);
    
    if(mode){   //cropping
        int cr = 0;
        if(w > 640) w = 640; cr = 1;
        if(h > 480) h = 480; cr = 1;
        if(cr) uprintf("Cropped to: width=%d heigh=%d", w, h);
    }
    else{   //resizing, 1/2, 1/3, 1/4.... until w <= 640 and h<= 480     
        while(w > 640 || h > 480){
            d++;
            w = w/d;
            h = h/d;    
        }
        if(d>1) uprintf("\nResized: width=%d heigh%d", w, h);    
    }
    for (i = start_row; i < start_row + h; i++){
        pp = p;
        for (j = start_col; j < start_col + w; j++){
            b = *pp; g = *(pp+1); r = *(pp+2);  //Valores RGB
            pixel = (b<<16) | (g<<8) | r;       //valor del pixel
            fb[i*WIDTH + j] = pixel;            //Escribe al frame buffer
            pp += (3*d);
        }
        p -= (rsize*d);     //linea anterior
    }
}

int main(){
    char c, *p;
    uart_init();    //inicializa UARTs
    UART *up = &uart[0];    //usa UART0
    fbuf_init();    //Modo VGA por defecto
    p = &_binary_image_start;
    while(1){
        uprintf("\nShowing cropped image");
        show_bmp(p, 0, 0, 1);     //show image cropped
        uprintf("\nPush any key to continue");
        ugetc(up);
        uprintf("\nShowing resized image");
        show_bmp(p, 0, 0, 0);    //shows image resized
        uprintf("\nPush any key to continue");
        ugetc(up);
    }
    while(1);   //entra en bucle infinito
}
