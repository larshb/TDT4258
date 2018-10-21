#include "waves.h"
#include "sound.h"

void (*waveFunctions[N_WAVES])(uint16_t) = {sawtooth, triangle, square};

static inline uint16_t calc_ticks_per_wave(uint16_t frequency) {
	return SAMPLE_RATE/(frequency)-1;
}

void sawtooth(uint16_t frequency) {
	//-----------------------------------------------v dirty fix
	//uint32_t ticks_per_wave = SAMPLE_RATE/(frequency)-1;//*2);
	snd_out=(snd_out+SND_OUT_MAX/calc_ticks_per_wave(frequency));
	if (snd_out > SND_OUT_MAX)
		snd_out = 0;
}

/* Somewhat broken */
void triangle(uint16_t frequency) {
	static uint8_t triangle_up_down_toggle = 0;
	static uint8_t triangle_counter = 0;
	//uint32_t ticks_per_wave2 = SAMPLE_RATE/(frequency)-1;//*2);
	uint16_t ticks_per_wave = calc_ticks_per_wave(frequency);
	if (triangle_counter++ == ticks_per_wave/2) {
		triangle_counter = 0;
		triangle_up_down_toggle ^= 1;
	}
	if (triangle_up_down_toggle)
		snd_out=(snd_out+SND_OUT_MAX/ticks_per_wave*2);
	else
		snd_out=(snd_out-SND_OUT_MAX/ticks_per_wave*2);
}


void square(uint16_t frequency){
	static uint16_t t = 0;
	//uint16_t ticks_per_wave2 = SAMPLE_RATE/(frequency)-1;
	uint16_t ticks_per_wave = calc_ticks_per_wave(frequency);
	t=(t+1)%ticks_per_wave;
	snd_out = t<ticks_per_wave/2?SND_OUT_MAX:0;
}
