#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define FB_PATH "/dev/fb0"
#define FB_UPDATE 0x4680

int screen_fd;
struct fb_copyarea screen_ca = {0, 0, 320, 240};
uint16_t* screen_ar;

uint16_t   RED = 0b1111100000000000;
uint16_t WHITE = 0b1111111111111111;
uint16_t  BLUE = 0b0000000000011111;

#define TRY(exp, str) \
	if ((exp) < 0) { \
		printf("ERROR: %s failed with error number %d\n", str, errno); \
		return errno; \
	}

int screen_init() {
	screen_fd = open(FB_PATH, O_RDWR);
	TRY(screen_fd, "Opening screen file");
   	screen_ar = mmap(NULL, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, screen_fd, 0);
	TRY(screen_ar, "Memory mapping screen")
   	return 0;
}

inline int screen_refresh() {
	TRY(ioctl(screen_fd, FB_UPDATE, &screen_ca), "Refreshing screen");
	return 0;
}

void draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint16_t r, c;
	for (r = x0; r < x1; r++) {
		for (c = y0; c < y1; c++) {
			screen_ar[c*320+r] = color;
		}
	}
}

void draw_flag() {
	draw_rectangle(   0,    0,   320,   240, RED);
	draw_rectangle(   0, 15*6,   320, 15*10, WHITE);
	draw_rectangle(15*6,    0, 15*10,   240, WHITE);
	draw_rectangle(   0, 15*7,   320,  15*9, BLUE);
	draw_rectangle(15*7,    0,  15*9,   240, BLUE);
}

void screen_test() {
	screen_init();
	draw_flag();
	screen_refresh();
}
