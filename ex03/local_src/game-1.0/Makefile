CFLAGS += -D_GNU_SOURCE -O2 -g -Wall -lm #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
CPPFLAGS += 
LDFLAGS += 
CC = arm-cortexm3-uclinuxeabi-gcc


EXECUTABLE	:= game

all: $(EXECUTABLE)

$(EXECUTABLE): *.c
	${CC} $(CFLAGS) $^ -o $@ ${LDFLAGS}

clean:
	-rm -f game

install:

rebuild:
	make clean
	make

.PHONY: all install clean rebuild
