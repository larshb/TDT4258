#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
(byte & 0x80 ? '1' : '0'), \
(byte & 0x40 ? '1' : '0'), \
(byte & 0x20 ? '1' : '0'), \
(byte & 0x10 ? '1' : '0'), \
(byte & 0x08 ? '1' : '0'), \
(byte & 0x04 ? '1' : '0'), \
(byte & 0x02 ? '1' : '0'), \
(byte & 0x01 ? '1' : '0') 



static int fd;

int gamepad_init() {
	if(system("mknod /dev/gamepad c 253 252") < 0)
		puts("System error...");
	if(system("modprobe driver-gamepad") < 0)
		puts("System error 2...");
   fd = open("/dev/gamepad", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
   	perror("Failed to open the device...");
   	return errno;
   }
   return 0;
}

inline uint8_t gamepad_read() {
	static uint8_t btn_in;
	if (read(fd, &btn_in, 1) < 0) {
		puts("Read error...");
	}
	return ~btn_in;
}

// typedef enum { SW1, SW2, SW3, SW4, SW5, SW6, SW7, SW8 } button_t;

// uint8_t gamepad_button_pressed(button_t btn) {
// 	/* FIXME Unnecessary to read from hardware every time */
// 	return ~(gamepad_read() >> (uint8_t)(btn)) & 1U;
// }

int gamepad_test(){
	int run;
	printf("Starting device test code example...\n");
	gamepad_init();
	printf("Starting program...\n");
	uint8_t btns;
	do {
		btns = gamepad_read();
		printf("Buttons: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(~btns));
		puts("Run again? [0=no, 1=yes]");
		fflush(stdin);
		if (scanf("%u", &run) < 0) {
			puts("Scan error...");
		}
		puts("");
	} while (run);

	return 0;
}
