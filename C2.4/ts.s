    .text
    .global start, sum
start:
    ldr sp, =stack_top //necesitamos el stack para realizar la llamada
    ldr r2, =a
    ldr r0, [r2]    //r0 = a
    ldr r2, =b
    ldr r1, [r2]    //r1 = b
    bl sum          //c = sum(a, b)
    ldr r2, =c
    str r0, [r2]    //guarda valor retornado c
stop: b stop
    .data
    a: .word 1
    b: .word 2
    c: .word 0
