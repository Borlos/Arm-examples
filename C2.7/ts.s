    .global reset_start
reset_start:
    ldr sp, =stack_top  //setea el puntero de stack de SVC
    bl main             //llama al main de C
    b .                  //Si vuelve de main se queda en loop infinito    
