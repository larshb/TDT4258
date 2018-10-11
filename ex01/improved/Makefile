LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=../efm32gg.ld

SOLUTION?=improved

ex1_${SOLUTION}.bin : ex1_${SOLUTION}.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1_${SOLUTION}.elf : ex1_${SOLUTION}.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

ex1_${SOLUTION}.o : ex1_${SOLUTION}.s
	${AS} ${ASFLAGS} $< -o $@

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex1_${SOLUTION}.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex