#include "efm32gg.h"
#include "sound.h"
#include "gpio.h"

void busy_sample_tick() {
	/* Wait approximately one timer counter cycle */
	while (*TIMER1_CNT < SAMPLE_PERIOD/2); // Wait
	while (*TIMER1_CNT > SAMPLE_PERIOD/2); // Wait
}

inline void busy_melody(melody_t* mel) {
	while(!snd_stepMelody(mel)){
		snd_audioOut();
		busy_sample_tick();
	}
}

void polling() {
	/* Play startup melody while last flag is not set */
	busy_melody(&mel_startup);

	while(1) {
		if (gpio_btn_pressed(SW6)) {
			busy_melody(&mel_powerup);
		}
		if (gpio_btn_pressed(SW7)) {
			busy_melody(&mel_1up);	
		}
		if (gpio_btn_pressed(SW8)) {
			busy_melody(&mel_powerdown);
		}
	}
}
