    .text
    .global start, sum
start:
    ldr sp, =stack_top //necesitamos el stack para realizar la llamada
    ldr r2, =a      //r2 apunta a 'a'
    ldr r0, [r2]    //r0 = a
    ldr r2, =b      //r2 apunta a 'b'
    ldr r1, [r2]    //r1 = b
    bl sum          //c = sum(a, b), el valor retornado irá a r0
    ldr r2, =c      //r2 apunta a c 
    str r0, [r2]    //guarda valor retornado (r0) en c
stop: b stop
    .data
    a: .word 1
    b: .word 2
    c: .word 0
