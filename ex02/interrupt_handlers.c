#include "efm32gg.h"
#include "sound.h"
#include "gpio.h"

void __attribute__ ((interrupt)) TIMER1_IRQHandler() {
	*TIMER1_IFC = 1U; /* Clear interrupt flag */

	snd_sampleTick();

	if (melody_done) {
		*TIMER1_CMD = 0; // Disable timer
		*SCR = 6U; // Sleep
	}
}

void GPIO_IRQHandler() {
	*GPIO_IFC = *GPIO_IF; /* Clear interrupt flag */
	*SCR = 4U; 		// Wake
	*TIMER1_CMD = 1; // Enable timer
	
	/* Volume control */
	if (gpio_btn_pressed(SW2)) {
		snd_volAdjust(UP);
		while(gpio_btn_pressed(SW2)); /* Debounce */
	}
	if (gpio_btn_pressed(SW4)){
		snd_volAdjust(DOWN);
		while(gpio_btn_pressed(SW4)); /* Debounce */
	}

	/* Display volume on leds */
	gpio_leds_write(0xFF >> (SND_VOL_MAX-snd_vol));

	/* Waveform select */
	if (gpio_btn_pressed(SW1)) {
		snd_waveSelect(PREVIOUS);
		while(gpio_btn_pressed(SW1)); /* Debounce */
	}
	if (gpio_btn_pressed(SW3)) {
		snd_waveSelect(NEXT);
		while(gpio_btn_pressed(SW3)); /* Debounce */
	}

	/* Sound effects */
	if (gpio_btn_pressed(SW5)){
		snd_selectMelody(&mel_pitchfork);
		while(gpio_btn_pressed(SW5)); /* Debounce */
	}
	if (gpio_btn_pressed(SW6)){
		snd_selectMelody(&mel_powerup);
		while(gpio_btn_pressed(SW6)); /* Debounce */
	}
	if (gpio_btn_pressed(SW7)){
		snd_selectMelody(&mel_1up);
		while(gpio_btn_pressed(SW7)); /* Debounce */
	}
	if (gpio_btn_pressed(SW8)){
		snd_selectMelody(&mel_powerdown);
		while(gpio_btn_pressed(SW8)); /* Debounce */
	}
}

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() {
	GPIO_IRQHandler();
}

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() {
	GPIO_IRQHandler();
}
