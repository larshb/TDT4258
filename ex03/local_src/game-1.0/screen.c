#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define FB_PATH "/dev/fb0"
#define FB_UPDATE_CMD 0x4680

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define BYTES_PER_PIXEL 2

int screen_fd;
struct fb_copyarea screen_ca = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
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
   	screen_ar = mmap(NULL, SCREEN_WIDTH*SCREEN_HEIGHT*BYTES_PER_PIXEL,
   				     PROT_READ | PROT_WRITE, MAP_SHARED, screen_fd, 0);
	TRY(screen_ar, "Memory mapping screen")
   	return 0;
}

inline int screen_refresh() {
	TRY(ioctl(screen_fd, FB_UPDATE_CMD, &screen_ca), "Refreshing screen");
	return 0;
}

void draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint16_t r, c;
	for (r = x0; r < x1; r++) {
		for (c = y0; c < y1; c++) {
			screen_ar[c*SCREEN_WIDTH+r] = color;
		}
	}
}

#define BLOCK SCREEN_HEIGHT/16
void draw_flag() {
	draw_rectangle(0,       0,       SCREEN_WIDTH, SCREEN_HEIGHT, RED);
	draw_rectangle(0,       6*BLOCK, SCREEN_WIDTH, 10*BLOCK,      WHITE);
	draw_rectangle(6*BLOCK, 0,       10*BLOCK,     SCREEN_HEIGHT, WHITE);
	draw_rectangle(0,       7*BLOCK, SCREEN_WIDTH, 9*BLOCK,       BLUE);
	draw_rectangle(7*BLOCK, 0,       9*BLOCK,      SCREEN_HEIGHT, BLUE);
}

void screen_test() {
	screen_init();
	draw_flag();
	screen_refresh();
}
