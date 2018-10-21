#ifndef WAVES_H__
#define WAVES_H__

#include <stdint.h>

#define N_WAVES 4

void sawtooth(uint16_t frequency);
void triangle(uint16_t frequency);
void square(uint16_t frequency);
void podium(uint16_t frequency);

void (*waveFunctions[N_WAVES])(uint16_t);

#endif /* end of include guard: WAVES_H__ */