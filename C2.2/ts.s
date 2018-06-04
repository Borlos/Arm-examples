/* 
Suma de un array de enteros 
Muestra el uso del stack para llamar a subrutinas
y demuestra modos de direccionaiento
*/

    .text
    .global start
start: ldr sp, =stack_top   //Inicia el puntero de stack
    bl sum                  //llama a sum
stop: b stop                //loop

sum: //int sum(): computa la suma de un array int en Resultado
    stmfd sp!, {r0-r4, lr}  //guarda lr y registros r-r4 al stack
    mov r0, #0          //r0 = 0
    ldr r1, =Array      //r1 = &Array
    ldr r2, =N          //r2 = &N
    ldr r2, [r2]        //r2 = N
loop: ldr r3, [r1], #4  //r3 = *(r1++)
    add r0, r0, r3      //r0 += r3
    sub r2, r2, #1      //r2--
    cmp r2, #0          //if (r2 != 0)
    bne loop            //goto loop;
    ldr r4, =Result     //r4 = &Result
    str r0, [r4]        //Result = r0
    ldmfd sp!, {r0-r4, pc}  //pop de pila, retorna al llamante

    .data
N:  .word 10 //Numero de elementos del array
Array:  .word 1,2,3,4,5,6,7,8,9,10
Result: .word 0
