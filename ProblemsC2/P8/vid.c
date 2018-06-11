#define RED 0
#define BLUE 1
#define GREEN 2
#define WHITE 3
#define OFFSET 1626     //My font file starts at (102*16) - 6
#define TAB 8
extern char _binary_font_start;
int color;
unsigned char cursor;
int volatile *fb;
int row, col, scroll_row;
unsigned char *font;
char *tab = "0123456789ABCDEF";
int WIDTH = 640; // scan line width, default to 640

int fbuf_init() {
    int i;
    fb = (int *)0x400000; // frame buffer at 2MB-4MB
    font = &_binary_font_start; // font bitmap
    /********* for 640x480 VGA mode *******************/
    *(volatile unsigned int *)(0x1000001c) = 0x2C77;
    *(volatile unsigned int *)(0x10120000) = 0x3F1F3F9C;
    *(volatile unsigned int *)(0x10120004) = 0x090B61DF;
    *(volatile unsigned int *)(0x10120008) = 0x067F1800;
    *(volatile unsigned int *)(0x10120010) = 0x400000; // at 2MB
    *(volatile unsigned int *)(0x10120018) = 0x82B;
    /********** for 800X600 SVGA mode ******************
    *(volatile unsigned int *)(0x1000001c) = 0x2CAC; // 800x600
    *(volatile unsigned int *)(0x10120000) = 0x1313A4C4;
    *(volatile unsigned int *)(0x10120004) = 0x0505F6F7;
    *(volatile unsigned int *)(0x10120008) = 0x071F1800;
    *(volatile unsigned int *)(0x10120010) = 0x200000;
    *(volatile unsigned int *)(0x10120018) = 0x82B;
    **********/
    cursor = 178; // cursor = row 127 in font bitmap
}

int clrpix(int x, int y){ // clear pixel at (x,y)
    int pix = y*640 + x;
    fb[pix] = 0x00000000;
}

int setpix(int x, int y){ // set pixel at (x,y)
    int pix = y*640 + x;
    if (color==RED) fb[pix] = 0x000000FF;
    if (color==BLUE) fb[pix] = 0x00FF0000;
    if (color==GREEN) fb[pix] = 0x0000FF00;
}

int dchar(unsigned char c, int x, int y){ // display char at (x,y)
    int r, bit;
    unsigned char *caddress, byte;
    caddress = font + c*16 + OFFSET;
    for (r=0; r<16; r++){
        byte = *(caddress + r);
        for (bit=0; bit<8; bit++){
            if (byte & (1<<bit))
            setpix(x+(7-bit), y+r); //My font file is x mirroed, so instead of x+bit I Have to put it in x+(7-bit)
        }
    }
}

int showFont (){    //Shows all the font file chars
    int x = 0, y = 0, bit, r, i;
    unsigned char *caddress, byte;
    for (i = 0; i < 254; i++){
        caddress = font + (i*16) + OFFSET;
        for( r=0; r<16; r++){
            byte = *(caddress + r);
            for (bit = 0; bit < 8; bit++){
                if (byte & (1<<bit)) setpix(x+(7-bit), y+r);
            }
        }
        x += 8;
        if (x > 600){
            x = 0; 
            y += 16;
        } 
    }
}

int undchar(unsigned char c, int x, int y){ // erase char at (x,y)
    int row, bit;
    unsigned char *caddress, byte;
    caddress = font + c*16 + OFFSET;
    for (row=0; row<16; row++){
        byte = *(caddress + row);
        for (bit=0; bit<8; bit++){
            if (byte & (1<<bit))
            clrpix(x+(7-bit), y+row);
        }
    }
}

int scroll(){ // scroll UP one line (the hard way)
    int i;
    for (i=64*640; i<640*480; i++){
        fb[i] = fb[i + 640*16];
    }
}

int kpchar(char c, int ro, int co){ // print char at (row, col)
    int x, y;
    x = co*8;
    y = ro*16;
    dchar(c, x, y);
}

int unkpchar(char c, int ro, int co){ // erase char at (row, col)
    int x, y;
    x = co*8;
    y = ro*16;
    undchar(c, x, y);
}

int erasechar(){ // erase char at (row,col)
    int r, bit, x, y;
    unsigned char *caddress, byte;
    x = col*8;
    y = row*16;
    for (r=0; r<16; r++){
        for (bit=0; bit<8; bit++){
            clrpix(x+(7-bit), y+r);
        }
    }
}

int clrcursor(){ // clear cursor at (row, col)
    unkpchar(cursor, row, col);
}

int putcursor(unsigned char c){ // set cursor at (row, col)
    kpchar(c, row, col);
}

int kputc(char c){ // print char at cursor position
    clrcursor();
    if (c == '\t'){
        col += TAB;
        putcursor(cursor);
        return;
    }
    if (c=='\r'){ // return key
        col=0;
        putcursor(cursor);
        return;
    }
    if (c=='\n'){ // new line key
        row++;
        if (row>=25){
            row = 24;
            scroll();
        }
        putcursor(cursor);
        return;
    }
    if (c=='\b'){ // backspace key
        if (col>0){
            col--;
            erasechar();
            putcursor(cursor);
        }
        return;
    }
    // c is ordinary char case
    kpchar(c, row, col);
    col++;
    if (col>=80){
        col = 0;
        row++;
        if (row >= 25){
            row = 24;
            scroll();
        }
    }
    putcursor(cursor);
}

// The following implements kprintf() for formatted printing
int kprints(char *s){
    while(*s){
        kputc(*s);
        s++;
    }
}

int krpx(int x){
    char c;
    if (x){
        c = tab[x % 16];
        krpx(x / 16);
    }
    kputc(c);
}

int kprintx(int x){
    kputc('0'); kputc('x');
    if (x==0) kputc('0');
    else krpx(x);
    kputc(' ');
}

int krpu(int x){
    char c;
    if (x){
        c = tab[x % 10];
        krpu(x / 10);
    }
    kputc(c);
}

int kprintu(int x){
    if (x==0) kputc('0');
    else krpu(x);
    kputc(' ');
}

int kprinti(int x){
    if (x<0){
        kputc('-');
        x = -x;
    }
    kprintu(x);
}

int kprintf(char *fmt, ...) {
    int *ip;
    char *cp;
    cp = fmt;
    ip = (int *)&fmt + 1;
    while(*cp){
        if (*cp != '%'){
            kputc(*cp);
            if (*cp=='\n') kputc('\r');
            cp++;
            continue;
        }
        cp++;
        switch(*cp){
            case 'c': kputc((char)*ip); break;
            case 's': kprints((char *)*ip); break;
            case 'd': kprinti(*ip); break;
            case 'u': kprintu(*ip); break;
            case 'x': kprintx(*ip); break;
        }
        cp++; ip++;
    }
}
