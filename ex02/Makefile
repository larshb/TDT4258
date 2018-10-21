INCLUDE= -I. -Ilib -Idev -Iaudio

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-mcpu=cortex-m3 -mthumb -g -std=c99 -Wall
LDFLAGS=-mcpu=cortex-m3 -mthumb -g -lgcc -lc -lcs3 -lcs3unhosted -lefm32gg -Llib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g
LINKERSCRIPT=lib/efm32gg.ld

bin/ex2.bin : bin/ex2.elf
	${OBJCOPY} -O binary $< $@

bin/ex2.elf : ex2.o dev/timer.o dev/dac.o dev/gpio.o \
	interrupt_handlers.o audio/waves.o audio/sound.o audio/melodies.o audio/notes.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

%.o : %.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

.PHONY : pretty
pretty :
	-indent *.c *.h

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "bin/ex2.bin" -r

.PHONY : clean
clean :
	-rm -rfv *.o */*.o */*.elf */*.bin */*.hex 

.PHONY : rebuild
rebuild :
	make clean
	make

