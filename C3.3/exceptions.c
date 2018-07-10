void UNDEF_handler(){
    kprintf("Hey! Error!, Undef");
    while(1);
}

void SWI_handler(){
    kprintf("Hey! Error!, SWI");
    while(1);
}

void PREFETCH_ABORT_handler(){
    kprintf("Hey! Error!, Prefetch");
    while(1);
}

void DATA_ABORT_handler(){
    kprintf("Hey! Error!, Data");
    while(1);
}

void FIQ_handler(){
    kprintf("Hey! Error!, FIQ");
    while(1);
}
