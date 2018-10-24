INCLUDE= -I. -Ibase -Iimproved -Icommon -Icommon/lib -Icommon/dev -Icommon/audio

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-mcpu=cortex-m3 -mthumb -g -std=c99 -Wall -Ofast
LDFLAGS=-mcpu=cortex-m3 -mthumb -g -lgcc -lc -lcs3 -lcs3unhosted -lefm32gg -Lcommon/lib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g
LINKERSCRIPT=common/lib/efm32gg.ld

SOURCES=$(shell find -name "*.c")
OBJECTS = $(SOURCES:.c=.o)
SOLUTION?=-DBASELINE

common/bin/ex2.bin : common/bin/ex2.elf
	${OBJCOPY} -O binary $< $@

common/bin/ex2.elf : common/ex2.o $(OBJECTS)
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

%.o : %.c
	${CC} ${CFLAGS} ${SOLUTION} ${INCLUDE} -c $< -o $@

.PHONY : pretty
pretty :
	-indent *.c *.h

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "common/bin/ex2.bin" -r

.PHONY : clean
clean :
	-rm -rfv *.o */*.o */*/*.o */*/*.elf */*/*.bin */*/*.hex 

.PHONY : rebuild
rebuild :
	make clean
	make

.PHONY : baseline
baseline :
	make clean
	make
	make upload

.PHONY : improved
improved :
	make clean
	make SOLUTION=-DIMPROVED
	make upload
