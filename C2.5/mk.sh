arm-none-eabi-as -o ts.o ts.s               # ensambla ts.s
arm-none-eabi-gcc -c t.c                    # compila t.c into t.o
arm-none-eabi-ld -T t.ld -o t.elf ts.o t.o  # enlaza t.elf 
arm-none-eabi-objcopy -O binary t.elf t.bin # convierte t.elf a t.bin

arm-none-eabi-readelf -a t.elf  #Muestra el elf generado

#qemu-system-arm -M versatilepb -m 128M -kernel t.bin -serial mon:stdio  #ejecuta t.bin en qemu
