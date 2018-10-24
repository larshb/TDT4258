#include "waves.h"
#include "sound.h"

/* TODO Use retvals instead of writing directly to snd_out? */

void (*waveFunctions[N_WAVES])(uint16_t) = {sawtooth, square, triangle, podium};

static inline uint16_t calc_ticks_per_wave(uint16_t frequency) {
	return SAMPLE_RATE/(frequency)-1;
}

#define GENERATOR_BASE(step_fn) \
	static uint16_t frequency_prev = 0; \
	static uint16_t ticks_per_wave = 0; \
	static uint8_t step = 0; \
	if (frequency_prev != frequency) { \
		frequency_prev = frequency; \
		ticks_per_wave = calc_ticks_per_wave(frequency); \
		step = (step_fn); \
	}

void sawtooth(uint16_t frequency) {
	GENERATOR_BASE(SND_OUT_MAX/ticks_per_wave);

	snd_out+=step;
	if (snd_out > SND_OUT_MAX)
		snd_out = 0;
}

/* Somewhat broken */
void triangle(uint16_t frequency) {
	GENERATOR_BASE(SND_OUT_MAX/ticks_per_wave*2)
	static uint8_t t = 0;

	if (t < ticks_per_wave/2+1)
		snd_out+=step;
	else
		snd_out-=step;

	t=(t+1)%ticks_per_wave;
}


void square(uint16_t frequency){
	GENERATOR_BASE(0)
	(void)step;
	static uint16_t t = 0;

	snd_out = t<ticks_per_wave/2?SND_OUT_MAX:0;

	t=(t+1)%ticks_per_wave;
}

void podium(uint16_t frequency) {
	GENERATOR_BASE(0)
	(void)step;
	static uint16_t t = 0;

	uint8_t quarter = t/(ticks_per_wave/4);
	switch(quarter) {
		case 0:
		default:
			snd_out = 0;
			break;
		case 1:
		case 3:
			snd_out = SND_OUT_MAX/2;
			break;
		case 2:
			snd_out = SND_OUT_MAX;
			break;
	}

	t=(t+1)%ticks_per_wave;
}
