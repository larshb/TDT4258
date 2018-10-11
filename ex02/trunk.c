void triangleForever(){
	volatile uint16_t i;
	uint16_t factor = 10;
	for (i = 0; i < 0xFFF; i+=factor){
		*DAC0_CH0DATA = i;
		*DAC0_CH1DATA = i;
	}
	for (i = 0xFFE; i > 0U; i-=factor){
		*DAC0_CH0DATA = i;
		*DAC0_CH1DATA = i;
	}
}

#include "mario.c"

typedef struct {
	uint8_t x;
	uint8_t n;
	uint16_t* y;
} waveform_t;

void playSample(waveform_t* wav) {
	snd_out = wav->y[wav->x];
	wav->x = (wav->x + 1) % wav->n;
	audioOut();
}

uint16_t sine480hz_y[100] = {2048, 2176, 2304, 2431, 2557, 2680, 2801, 2919, 3034, 3145, 3251, 3353, 3449, 3540, 3625, 3704, 3776, 3842, 3900, 3951, 3995, 4031, 4059, 4079, 4091, 4095, 4091, 4079, 4059, 4031, 3995, 3951, 3900, 3842, 3776, 3704, 3625, 3540, 3449, 3353, 3251, 3145, 3034, 2919, 2801, 2680, 2557, 2431, 2304, 2176, 2048, 1919, 1791, 1664, 1538, 1415, 1294, 1176, 1061, 950, 844, 742, 646, 555, 470, 391, 319, 253, 195, 144, 100, 64, 36, 16, 4, 0, 4, 16, 36, 64, 100, 144, 195, 253, 319, 391, 470, 555, 646, 742, 844, 950, 1061, 1176, 1294, 1415, 1538, 1664, 1791, 1919};
waveform_t sine480hz = {
	.y = sine480hz_y,
	.x = 0,
	.n = 100
};

waveform_t fireball = {
	.y = fireball_y,
	.x = 0,
	.n = 9672
};

uint16_t random_y[9] = {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000};//, 3000, 2000, 1000};
waveform_t random = {
	.y = random_y,
	.x = 0,
	.n = 9
};

void sawtooth_simple() {
	unsigned int factor = 1;
	snd_out=(snd_out+factor)%snd_vol;
	*DAC0_CH0DATA = snd_out;
	*DAC0_CH1DATA = snd_vol-snd_out;
	busy_sample_tick();
}

void square_simple(){
	static int i = 0;
	i=(i+1)%100;
	snd_out = i<50?0:SND_VOL_MAX;
}