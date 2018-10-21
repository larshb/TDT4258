#ifndef SOUND_H__
#define SOUND_H__

#include "melodies.h"

#include <stdint.h>

#define SAMPLE_RATE_REQ	   48000 /* Requested sample rate */
#define DAC_CLK			14000000 /* Default DAC clock frequency [Hz] */
#define SAMPLE_PERIOD 	(DAC_CLK/SAMPLE_RATE_REQ) 	/* ~ 48 kHz (ticks / 14MHz [default]) */
#define SAMPLE_RATE 	(DAC_CLK/(uint16_t)SAMPLE_PERIOD) /* Actual sample rate (floored) */

#define SND_OUT_MAX 0xFFF 	/* 12 bit channel registers */
#define SND_VOL_MAX 8		/* Volume adjustment resolution */

volatile uint16_t snd_out;  /* Audio channel request register (mono) */
volatile uint8_t  snd_vol;	/* Current volume adjustment factor */

typedef enum {UP, DOWN} snd_vol_adj_t;

void snd_init();

void snd_volAdjust(snd_vol_adj_t adj);

void snd_audioOut();

uint8_t snd_PlayMelody(melody_t* melody);

#endif /* end of include guard: SOUND_H__ */
