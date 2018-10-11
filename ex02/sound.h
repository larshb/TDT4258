#ifndef SOUND_H__
#define SOUND_H__

#include <stdint.h>

/*
 * The period between sound samples, in clock cycles 
 */
//#define	  FUDGE_FACTOR 2
#define   SAMPLE_RATE 48000
#define   DAC_CLK 14000000
#define   SAMPLE_PERIOD (uint16_t)(DAC_CLK/SAMPLE_RATE) //292 /* ~ 48 kHz (ticks / 14MHz [default]) */

#define SND_OUT_MAX 0xFFF
#define SND_VOL_MAX 10
volatile uint32_t snd_out;
volatile uint32_t snd_vol;

typedef enum {UP, DOWN} snd_vol_adj_t;

void snd_init();

void snd_volAdjust(snd_vol_adj_t adj);

void audioOut();

#endif /* end of include guard: SOUND_H__ */
