#ifndef SCREEN_H__
#define SCREEN_H__

#include <stdint.h>

/* 16-bit colors */

#define  WHITE 0b1111111111111111
#define  BLACK 0b0000000000000000

#define    RED 0b1111100000000000
#define YELLOW 0b1111111111100000
#define  GREEN 0b0000011111100000
#define   CYAN 0b0000011111111111
#define   BLUE 0b0000000000011111
#define PURPLE 0b1111100000011111

#define   SKIN 0b1111111001010110

int screen_init();
inline int screen_refresh();
void screen_clear();
void draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

#endif /* end of include guard: SCREEN_H__ */