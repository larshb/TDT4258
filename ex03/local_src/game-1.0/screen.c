#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>

#define SCREEN_ROWS 240

int fbfd;
struct fb_copyarea rect;

typedef struct {
	uint8_t r;//:5;
	uint8_t g;//:5;
	uint8_t b;//:6;
} pixel_t;

void pixel_merge(uint16_t* out, pixel_t* pixels, uint32_t n) {
	uint32_t i;
	for (i = 0; i < n; i++) {
		pixel_t p = pixels[i];
		out[i] = \
		  ((p.r << 11) & 0b1111100000000000) \
		| ((p.g << 5)  & 0b0000011111100000) \
		| ((p.b)       & 0b0000000000011111);
		printf("R:%x G:%x B:%x out:%x\n", p.r, p.g, p.b, out[i]);
	}
}

int screen_init() {
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0){
      perror("Failed to open frame buffer file...");
      return errno;
   	}
   	return 0;
}

int init_framebuffer();

inline void screen_refresh() {
	ioctl(fbfd, 0x4680, &rect);
}

void screen_test() {
	//init_framebuffer();
	//ioctl(fbfd, 0x4680, &rect);
	//return;
	pixel_t red = {31, 0, 0};
	uint16_t red_out;
	pixel_merge(&red_out, &red, 1);
	printf("Red: %x\n", red_out);
	screen_init();
	

	pixel_t pixels[] = {
		{.r=31, 0, 0},
		{ 0,31, 0},
		{ 0, 0,31},
		{31, 0, 0},
		{ 0,31, 0},
		{ 0, 0,31},
		{31, 0, 0},
		{ 0,31, 0},
		{ 0, 0,31},
		{31, 0, 0},
		{ 0,31, 0},
		{ 0, 0,31}
	};
	int i, j;
	uint16_t pixels_out[12];
	pixel_merge(pixels_out, pixels, 12);
	/*uint16_t screen[320*6];
	for (i = 0; i < 320*6; i+=12){
		for (j = 0; j < 12; j++) {
			screen[i+j]=pixels_out[j];
		}
	}
	write(fbfd, (char*)screen, 320*6*2);*/

	uint16_t* screen = mmap(NULL, 320*SCREEN_ROWS*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (screen < 0) {
		printf("mmap returned error %d", errno);
		return;
	}/*
	screen[0] = 0xFFFF;
	screen[1] = 0xFFFF;
	screen[2] = 0xFFFF;
	screen[3] = 0xFFFF;
	screen[4] = 0xFFFF;
	screen[5] = 0xFFFF;
	screen[6] = 0xFFFF;
	screen[7] = 0xFFFF;
	screen[8] = 0xFFFF;*/
	for (i = 0; i < 320*SCREEN_ROWS/12; i++) {
		for (j = 0; j < 12; j++) {
			screen[i*12+j] = i;//pixels_out[j];
		}
	}
	screen_refresh();
}

uint16_t* fbp;
int screensize_pixels;
int screensize_bytes;

struct fb_var_screeninfo vinfo;
struct fb_copyarea screen;

int init_framebuffer()
{
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        printf("Error: unable to open framebuffer device.\n");
        return -1;
    }

    // Get screen size info
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        printf("Error: unable to get screen info.\n");
        return -1;
    }
    printf("Screeninfo: %d x %d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    screen.dx = 0;
    screen.dy = 0;
    screen.width = vinfo.xres;
    screen.height = vinfo.yres;

    screensize_pixels = vinfo.xres * vinfo.yres;
    screensize_bytes = screensize_pixels * vinfo.bits_per_pixel / 8;

    fbp = (char*)mmap(0, screensize_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (fbp == MAP_FAILED) {
        printf("Error: failed to memorymap framebuffer.\n");
        return -1;
    }

    int i;
    for (i = 0; i < screensize_pixels; i++) {
        fbp[i] = 0xFFFF;
        ioctl(fbfd, 0x4680, &rect);
    }


    char command[250*320];
    sprintf(command, "echo %s > /dev/fb0", (char*)fbp);
    system(command);

    return 0;
}