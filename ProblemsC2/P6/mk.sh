arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i1 i1.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i2 i2.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i3 i3.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i4 i4.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i5 i5.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i6 i6.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i7 i7.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i8 i8.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i9 i9.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i10 i10.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i11 i11.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i12 i12.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i13 i13.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i14 i14.o
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm i15 i15.o

arm-none-eabi-as -mcpu=arm926ej-s ts.s -o ts.o               # ensambla ts.s
arm-none-eabi-gcc -c -mcpu=arm926ej-s t.c -o t.o                    # compila t.c into t.o
arm-none-eabi-ld -T t.ld ts.o t.o i1.o i2.o i3.o i4.o i5.o i6.o i7.o i8.o i9.o i10.o i11.o i12.o i13.o i14.o i15.o -Llib -lgcc -o t.elf  # enlaza t.elf 
arm-none-eabi-objcopy -O binary t.elf t.bin # convierte t.elf a t.bin
echo ready to go?
read dummy

arm-none-eabi-readelf -a t.elf  #Muestra el elf generado

qemu-system-arm -M versatilepb -m 128M -kernel t.bin -serial mon:stdio  #ejecuta t.bin en qemu
