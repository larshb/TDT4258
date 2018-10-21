#ifndef SOUND_H__
#define SOUND_H__

#include "melodies.h"

#include <stdint.h>

#define SAMPLE_RATE_REQ	   44100 /* Requested sample rate */
#define DAC_CLK			14000000 /* Default DAC clock frequency [Hz] */
#define SAMPLE_PERIOD 	(DAC_CLK/SAMPLE_RATE_REQ) 	/* ~ 48 kHz (ticks / 14MHz [default]) */
#define SAMPLE_RATE 	(DAC_CLK/(uint16_t)SAMPLE_PERIOD) /* Actual sample rate (floored) */

#define SND_OUT_MAX 0xFFF 	/* 12 bit channel registers */
#define SND_VOL_MAX 8		/* Volume adjustment resolution */

volatile uint16_t snd_out;  /* Audio channel request register (mono) */
extern uint8_t  snd_vol;	/* Current volume adjustment factor */

extern uint8_t melody_done;
extern melody_t* melody_ptr;

typedef enum {UP, DOWN} snd_vol_adj_t;

void snd_init();

void snd_sampleTick();

void snd_volAdjust(snd_vol_adj_t adj);

void snd_audioOut();

typedef enum {PREVIOUS, NEXT} snd_waveSelectDir_t;
void snd_waveSelect(snd_waveSelectDir_t dir);

uint8_t snd_stepMelody(melody_t* melody); /* Iterate melody samples */

void snd_selectMelody(melody_t* mel); /* (Re)play a selected melody */

#endif /* end of include guard: SOUND_H__ */
