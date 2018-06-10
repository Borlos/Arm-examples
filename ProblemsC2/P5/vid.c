
int volatile *fb;
int WIDTH = 640;

int fbuf_init(){
    fb = (int *)(0x400000); //de 2 a 4MB
    //*************** para 640x480 VGA ****************/
    
    *(volatile unsigned int *)(0x1000001c) = 0x2C77;
    *(volatile unsigned int *)(0x10120000) = 0x3F1F3F9C;
    *(volatile unsigned int *)(0x10120004) = 0x090B61DF;
    *(volatile unsigned int *)(0x10120008) = 0x067F1800;
    
    /***************** para 800X600 SVGA ****************
    else{
    *(volatile unsigned int *)(0x1000001c) = 0x2CAC;
    *(volatile unsigned int *)(0x10120000) = 0x1313A4C4;
    *(volatile unsigned int *)(0x10120004) = 0x0505F6F7;
    *(volatile unsigned int *)(0x10120008) = 0x071F1800;
    }
    ****************************************************/
    *(volatile unsigned int *)(0x10120010) = 0x400000; // frame buffer
    *(volatile unsigned int *)(0x10120018) = 0x82B;
}

