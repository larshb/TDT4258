LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=../efm32gg.ld

ex1_polling.o : ex1_polling.s
	${AS} ${ASFLAGS} $< -o $@

ex1_polling.elf : ex1_polling.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}

ex1_polling.bin : ex1_polling.elf
	${OBJCOPY} -j .text -O binary $< $@



.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex1_polling.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex
