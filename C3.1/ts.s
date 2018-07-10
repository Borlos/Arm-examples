    .text
    .code 32
    .global reset_handler, vectors_start, vectors_end

reset_handler:
    LDR sp, =svc_stack_top  //set SVC mode stack
    BL copy_vectors         //copy vector table to adress 0
    MSR cpsr, #0x92         //to IRQ mode
    LDR sp, =irq_stack_top  //set IRQ mode stacj
    MSR cpsr, #0x13         //go back to SVC mode with IRQ on
    BL main                 //call main() in C
    B .                     //loop if main ever return

irq_handler:
    sub lr, lr, #4          //adjust lr (r14) for return, (PC-4)
    stmfd sp!, {r0-r12, lr} //stack r0-r12 and lr
    bl IRQ_handler          //call to IRQ_handler() in C
    ldmfd sp!, {r0-r12,pc}^ //return

undef_handler:
    sub lr, lr, #4          //adjust lr (r14) for return, (PC-4)
    stmfd sp!, {r0-r12, lr} //stack r0-r12 and lr
    bl UNDEF_handler          //call to IRQ_handler() in C
    ldmfd sp!, {r0,r12,pc}^ //return

swi_handler:
    sub lr, lr, #4          //adjust lr (r14) for return, (PC-4)
    stmfd sp!, {r0-r12, lr} //stack r0-r12 and lr
    bl SWI_handler          //call to IRQ_handler() in C
    ldmfd sp!, {r0,r12,pc}^ //return
 
prefetch_abort_handler:
    sub lr, lr, #4          //adjust lr (r14) for return, (PC-4)
    stmfd sp!, {r0-r12, lr} //stack r0-r12 and lr
    bl PREFETCH_ABORT_handler          //call to IRQ_handler() in C
    ldmfd sp!, {r0,r12,pc}^ //return

data_abort_handler:
    sub lr, lr, #4          //adjust lr (r14) for return, (PC-4)
    stmfd sp!, {r0-r12, lr} //stack r0-r12 and lr
    bl DATA_ABORT_handler          //call to IRQ_handler() in C
    ldmfd sp!, {r0,r12,pc}^ //return
 
fiq_handler:
    sub lr, lr, #4          //adjust lr (r14) for return, (PC-4)
    stmfd sp!, {r0-r12, lr} //stack r0-r12 and lr
    bl FIQ_handler          //call to IRQ_handler() in C
    ldmfd sp!, {r0,r12,pc}^ //return

vectors_start: //Interrupt-Exception addresses, from 0 in memory, after an interrupt the program will chech here where to go
    LDR PC, reset_handler_addr
    LDR PC, undef_handler_addr
    LDR PC, swi_handler_addr
    LDR PC, prefetch_abort_handler_addr
    LDR PC, data_abort_handler_addr
    B .
    LDR PC, irq_handler_addr
    LDR PC, fiq_handler_addr 
    reset_handler_addr:         .word reset_handler
    undef_handler_addr:         .word undef_handler
    swi_handler_addr:           .word swi_handler
    prefetch_abort_handler_addr:.word prefetch_abort_handler
    data_abort_handler_addr:    .word data_abort_handler
    irq_handler_addr:           .word irq_handler
    fiq_handler_addr:           .word fiq_handler
vectors_end:
