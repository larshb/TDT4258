#include "sound.h"
#include "efm32gg.h"

void snd_init(){
	snd_vol = 5;
}

void snd_volAdjust(snd_vol_adj_t adj) {
	if (adj == UP) {
		snd_vol = snd_vol<SND_VOL_MAX?(snd_vol+1):snd_vol;
	}
	else
		snd_vol = snd_vol == 0 ? 0 : (snd_vol - 1);
}

void audioOut() {
	if (snd_out > SND_OUT_MAX)
		snd_out = SND_OUT_MAX;
	uint32_t amp = (uint32_t)(snd_out*snd_vol)/SND_VOL_MAX;
	//uint16_t amp = snd_out;
	*DAC0_CH0DATA = amp;
	*DAC0_CH1DATA = amp;
}
