LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld

ex1.bin : ex1_interrupt.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1.elf : ex1_interrupt.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}

ex1.o : ex1_interrupt.s
	${AS} ${ASFLAGS} $< -o $@

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex1_interrupt.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex
