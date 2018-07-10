arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm font font.o

arm-none-eabi-as -mcpu=arm926ej-s ts.s -o ts.o               # ensambla ts.s
arm-none-eabi-gcc -c -mcpu=arm926ej-s t.c -o t.o                    # compila t.c into t.o
arm-none-eabi-ld -T t.ld ts.o t.o font.o -o t.elf  # enlaza t.elf 
arm-none-eabi-objcopy -O binary t.elf t.bin # convierte t.elf a t.bin
echo ready to go?
read dummy

arm-none-eabi-readelf -a t.elf  #Muestra el elf generado

qemu-system-arm -M versatilepb -m 256M -kernel t.bin -serial mon:stdio  #ejecuta t.bin en qemu
