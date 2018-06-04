    .global start, stack_top    //Stack definido en t.ld
start:
    ldr sp, =stack_top  //setea el puntero de stack de SVC
    bl main             //llama al main de C
    b .                  //Si vuelve de main se queda en loop infinito    
