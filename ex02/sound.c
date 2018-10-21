#include "sound.h"
#include "efm32gg.h"
#include "waves.h"

#define MELODY_SPS 1000 /* Steps per second / tone length resolution */

/* NOTE This seemed to slow performance slightly */
void (*snd_waveFcn)(uint16_t) = sawtooth; /* Waveform generator function */

/* FIXME Make flag and melody pointer atomic to avoid race conditions */
uint8_t melody_done = 0;
melody_t* melody_ptr = &mel_startup;

void snd_playMelody();

uint8_t  snd_vol = 5;	/* Current volume adjustment factor */

void snd_init(){
	/* Nothing here */
}

void snd_sampleTick() {
	/* Output sample at start of interrupt to ensure smooth playback */
	snd_audioOut();

	/* Play startup sound */
	snd_playMelody();

	/* Do as little as possible inside this ISR to make sure no
	 * samples are missed or corrupted.
	 */
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

	/* Write to audio channel registers */
	*DAC0_CH0DATA = amp;
	*DAC0_CH1DATA = amp;
}

void snd_waveSelect(snd_waveSelectDir_t dir) {
	static int8_t wav_itr = 0;
	/* Wraps around (eg. 0->1->2->0->1->2->...) */
	switch(dir) {
		case PREVIOUS:
			wav_itr = wav_itr == 0 ? N_WAVES-1 : wav_itr-1;
			break;
		case NEXT:
			wav_itr = wav_itr == N_WAVES-1 ? 0 : wav_itr+1;
			break;
		default:
			break;
	}
	//wav_itr%=N_WAVES;
	snd_waveFcn = waveFunctions[wav_itr];
}

uint8_t snd_stepMelody(melody_t* melody) {
	uint8_t last = 0;
	static uint32_t time_iterator = 0;

	/* Overlap insurance */
	static melody_t* prev_mel = 0;
	if (prev_mel != melody) {
		prev_mel = melody;
		time_iterator = 0;
	}

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
	snd_waveFcn(note2freq(melody->tones[melody->x].note));

	return last;
}

void snd_playMelody() {
	if (!melody_done) {
		melody_done = snd_stepMelody(melody_ptr);
	}
}

void snd_selectMelody(melody_t* mel) {
	melody_done = 0;
	melody_ptr = mel;
}
