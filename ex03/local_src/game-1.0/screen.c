#include "screen.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
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

inline int screen_refresh_area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	struct fb_copyarea area = {x0, y0, x1, y1};
	TRY(ioctl(screen_fd, FB_UPDATE_CMD, &area), "Refreshing screen");
	return 0;
}

/* Testing features: */

void draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint16_t r, c;
	for (c = x0; c < x1; c++) {
		for (r = y0; r < y1; r++) {
			screen_ar[r*SCREEN_WIDTH+c] = color;
		}
	}
	screen_refresh_area(x0, y0, x1-x0, y1-y0);
}

/* Draw block of data from upper-left [x/y] with dimensions [dx/dy] */
void draw_block(uint16_t* data, uint16_t x, uint16_t y, uint16_t dx, uint16_t dy) {
	uint16_t r, c;
	for (c = 0; c < dx; c++) {
		for (r = 0; r < dy; r++) {
			screen_ar[(r+y)*SCREEN_WIDTH+(c+x)] = data[r*dx+c];
		}
	}
	screen_refresh_area(x, y, dx, dy);
}

#define ABS(a) (a<0?-a:a)
void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color) {
	int16_t step;
	int16_t dx = x1-x0;
	int16_t dy = y1-y0;
	step = (ABS(dx)>ABS(dy)) ? ABS(dx) : ABS(dy);
	float dxf = (float)dx/step;
	float dyf = (float)dy/step;
	float x = x0;
	float y = y0;
	int16_t i = 0;
	while (i++ < step) {
		screen_ar[(int)y*SCREEN_WIDTH+(int)x] = color;
		x = x+dxf;
		y = y+dyf;
	}
}

void screen_clear() {
	draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
}

#define BLOCK SCREEN_HEIGHT/16
void draw_flag() {
	draw_rectangle(0,       0,       SCREEN_WIDTH, SCREEN_HEIGHT, RED);
	draw_rectangle(0,       6*BLOCK, SCREEN_WIDTH, 10*BLOCK,      WHITE);
	draw_rectangle(6*BLOCK, 0,       10*BLOCK,     SCREEN_HEIGHT, WHITE);
	draw_rectangle(0,       7*BLOCK, SCREEN_WIDTH, 9*BLOCK,       BLUE);
	draw_rectangle(7*BLOCK, 0,       9*BLOCK,      SCREEN_HEIGHT, BLUE);
}

#define _O(character, color) case character: o = color; break
void cdraw(uint16_t* out, char* in, uint16_t n) {
	uint16_t i, o;
	for (i = 0; i < n; i++) {
		switch (in[i]) {
			_O('W', WHITE);
			_O('B', BLACK);
			_O('R', RED);
			_O('S', SKIN);
			default: o=WHITE; break;
		}
		out[i] = o;
	}
}

int8_t sine[40] = {0,15,30,45,58,70,80,89,95,98,100,98,95,89,80,70,58,45,30,15,0,-16,-31,-46,-59,-71,-81,-90,-96,-99,-100,-99,-96,-90,-81,-71,-59,-46,-31,-16};
int8_t cosine[40] = {100,98,95,89,80,70,58,45,30,15,0,-16,-31,-46,-59,-71,-81,-90,-96,-99,-100,-99,-96,-90,-81,-71,-59,-46,-31,-16,-1,15,30,45,58,70,80,89,95,98};

typedef struct {
	uint16_t x, y;
} coords_t;

void animate_diamond() {
	int t;
	for (;;) {
		for (t = 0; t < 40; t++) {
			screen_clear();
			coords_t a, b, c, d, e, f;
			a.x = 160-cosine[t]/5;
			a.y = 10;
			b.x = 160+cosine[t];
			b.y = 100;
			c.x = 160+cosine[t]/5;
			c.y = 230;
			d.x = 160-cosine[t];
			d.y = 140;
			e.x = 160+sine[t];
			e.y = 120;
			f.x = 160-sine[t];
			f.y = 120;
			draw_line(a.x, a.y, b.x, b.y, YELLOW);
			draw_line(b.x, b.y, c.x, c.y, YELLOW);
			draw_line(a.x, a.y, d.x, d.y, YELLOW);
			draw_line(d.x, d.y, c.x, c.y, YELLOW);
			draw_line(a.x, a.y, e.x, e.y, YELLOW);
			draw_line(e.x, e.y, c.x, c.y, YELLOW);
			draw_line(a.x, a.y, f.x, f.y, YELLOW);
			draw_line(f.x, f.y, c.x, c.y, YELLOW);
			draw_line(b.x, b.y, e.x, e.y, YELLOW);
			draw_line(e.x, e.y, d.x, d.y, YELLOW);
			draw_line(d.x, d.y, f.x, f.y, YELLOW);
			draw_line(f.x, f.y, b.x, b.y, YELLOW);
			screen_refresh();
		}
	}
}

void draw_gridlines() {
	int r, c;
	for (r = 0; r < 320; r+=2) {
		for (c = 0; c < 240; c+=2) {
			draw_line(0,c,r,c, RED); //UL
			draw_line(r,0,r,c, BLUE); //UL
		}
		screen_refresh();
	}
}

void draw_corners() {
	draw_line(0,0,10,10,RED); //UL
	draw_line(309,10,319,0, BLACK); //UR
	draw_line(0,239,10,229, BLUE); //LL
	draw_line(319,239,309,229, SKIN); //LR
}

void draw_mario() {
	char mario_str[16*16] = {"WWWWWBBBBBWWWWWWWWWWBRRRRRBBBWWWWWWBRRRRRRRRRBWWWWWBBBSSBSBBBWWWWWBSSBBSBSBBBWWWWWBSSBBBSSBBBWWWWWWBBSSSBBBBBWWWWWWBBBSSSSSBWWWWWBBRRRBBRRBBBWWWBSSRRRBBRRBRBWWWBSSSRRBBBBBBRSBWWBSSBBBBSBBSBSBWWWBBBBBBBBBRRBWWWBRRBBBBBBRRRBWWWBRRRBWWBRRRBWWWWWBBBWWWWBBBWWWW"};
	uint16_t mario[16*16];
	cdraw(mario, mario_str, 16*16);
	draw_block(mario, 160, 120, 16, 16);
}

uint16_t* font_getsymbol(char c);

/* Print characters based on 8x8 grid */
void screen_writechar(char c, uint8_t x, uint8_t y) {
	draw_block(font_getsymbol(c), x*8, y*8, 8, 8);
}

void screen_print(char* str, uint8_t x, uint8_t y) {
	char* c = str;
	do {
		if (*c == '\n') {
			y++;
		}
		else {
			screen_writechar(*c, x++, y);
		}
	} while (*(++c) != 0);
}

void ascii_table() {
	uint8_t r, c;
	for (r = 0; r <= 8; r++) {
		for (c = 0; c <= 9; c++) {
			screen_writechar(r*9+c, r, c);
		}
	}
}

void darth_plagueis() {
	int r = 1;
	screen_print("Did you ever hear the tragedy of Darth",0,r++);
	screen_print("Plagueis the Wise? I thought not. It's",0,r++);
	screen_print("not a story the Jedi would tell you.  ",0,r++);
	screen_print("It's a Sith legend. Darth Plagueis was",0,r++);
	screen_print("a Dark Lord of the Sith, so powerful  ",0,r++);
	screen_print("and so wise he could use the Force to ",0,r++);
	screen_print("influence the midichlorians to create ",0,r++);
	screen_print("life... He had such a knowledge of the",0,r++);
	screen_print("dark side that he could even keep the ",0,r++);
	screen_print("ones he cared about from dying. The   ",0,r++);
	screen_print("dark side of the Force is a pathway to",0,r++);
	screen_print("many abilities some consider to be    ",0,r++);
	screen_print("unnatural. He became so powerful...   ",0,r++);
	screen_print("the only thing he was afraid of was   ",0,r++);
	screen_print("losing his power, which eventually, of",0,r++);
	screen_print("course, he did. Unfortunately, he     ",0,r++);
	screen_print("taught his apprentice everything he   ",0,r++);
	screen_print("knew, then his apprentice killed him  ",0,r++);
	screen_print("in his sleep. It's ironic he could    ",0,r++);
	screen_print("save others from death, but not       ",0,r++);
	screen_print("himself.                              ",0,r++);
}

void font_init();

void screen_test() {
	screen_init();
	font_init();
	//animate_diamond();
	//ascii_table();

	//draw_mario();

	//screen_print("Hade bra, Didrik!", 5, 5);

	darth_plagueis();

	screen_refresh();
}
