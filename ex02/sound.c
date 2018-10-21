#include "sound.h"
#include "efm32gg.h"
#include "waves.h"

#define MELODY_SPS 1000 /* Steps per second / tone length resolution */

void snd_init(){
	snd_vol = 5;
}

void snd_volAdjust(snd_vol_adj_t adj) {
	if (adj == UP) {
		snd_vol = snd_vol<SND_VOL_MAX ? (snd_vol+1) : snd_vol;
	}
	else
		snd_vol = snd_vol == 0 ? 0 : (snd_vol - 1);
}

void snd_audioOut() {
	if (snd_out > SND_OUT_MAX)
		snd_out = SND_OUT_MAX; /* Saturate output (12 bits) */
	uint32_t amp = (uint32_t)(snd_out*snd_vol)/SND_VOL_MAX; /* Apply volume */
	//uint16_t amp = snd_out;
	*DAC0_CH0DATA = amp;
	*DAC0_CH1DATA = amp;
}

uint8_t snd_PlayMelody(melody_t* melody) {
	uint8_t last = 0;
	static uint32_t time_iterator = 0;

	if (time_iterator == SAMPLE_RATE/MELODY_SPS*melody->tones[melody->x].duration) {
		time_iterator = 0;
		if (melody->x < melody->n - 1) {
			melody->x++;
		}
		else {
			melody->x = 0;
			last = 1;
		}
	}

	time_iterator++;
	square(melody->tones[melody->x].frequency);

	return last;
}

uint8_t snd_PlayMelody2(melody_t2* melody) {
	uint8_t last = 0;
	static uint32_t time_iterator = 0;

	if (time_iterator == SAMPLE_RATE/MELODY_SPS*melody->tones[melody->x].duration) {
		time_iterator = 0;
		if (melody->x < melody->n - 1) {
			melody->x++;
		}
		else {
			melody->x = 0;
			last = 1;
		}
	}

	time_iterator++;
	sawtooth(note2freq(melody->tones[melody->x].note));

	return last;
}
