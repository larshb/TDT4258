#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>

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
