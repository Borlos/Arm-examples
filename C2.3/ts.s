    .global start, sum
start: ldr sp, =stack_top   //declara pila
    bl main     //llama funcion main de C
stop: b stop

sum:    //int sum(a,b,c,d,e,f) llamada desde C

//al entrar, la parte alta del stack contiene e,f, pasados main en C
    stmfd sp!, {fp, lr}     //Establece marco de stacj con push fp, lr
    add fp, sp, #4          //Fp -> guarda lr en stack

//Computa la suma de parámetros que va haciendo en r0
    add r0, r0, r1
    add r0, r0, r2
    add r0, r0, r3
    ldr r3, [fp, #4]        //Carga parametro 'e' en r3 [fp+4]
    add r0, r0, r3          //lo suma a r0
    ldr r3, [fp, #8]        //Carga parametro 'f' en r3 [fp+8]
    add r0, r0, r3          //lo suma a r0
//Retorna al llamante
    sub sp, fp, #4          //sp = fp-4 (apunta al fp guardado)
    ldmfd sp!, {fp, pc}     //vuelve al llamante
