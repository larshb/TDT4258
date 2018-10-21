#include "waves.h"
#include "sound.h"

void (*waveFunctions[N_WAVES])(uint16_t) = {sawtooth, triangle, square};

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
	GENERATOR_BASE(1)

	static uint16_t t = 0;

	t=(t+1)%ticks_per_wave;
	snd_out = t<ticks_per_wave/2?step*SND_OUT_MAX:0;
}
