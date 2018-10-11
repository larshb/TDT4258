#include "waves.h"
#include "sound.h"

void sawtooth(uint16_t frequency) {
	//-----------------------------------------------v dirty fix
	uint32_t ticks_per_wave = SAMPLE_RATE/(frequency*2);
	snd_out=(snd_out+SND_OUT_MAX/ticks_per_wave);
	if (snd_out > SND_OUT_MAX)
		snd_out = 0;
}

uint8_t triangle_up_down_toggle = 0;
uint8_t triangle_counter = 0;
void triangle(uint16_t frequency) {
	//-----------------------------------------------v dirty fix (???)
	uint32_t ticks_per_wave = SAMPLE_RATE/(frequency);//*2);
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
	//-----------------------------------------------v dirty fix
	uint16_t ticks_per_wave = SAMPLE_RATE/(frequency*2);
	t=(t+1)%ticks_per_wave;
	snd_out = t<ticks_per_wave/2?SND_OUT_MAX:0;
}
