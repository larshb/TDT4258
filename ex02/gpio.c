#include <stdbool.h>

#include "efm32gg.h"

#include "gpio.h"

#include "sound.h"
#include "melodies.h"

/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO()
{
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL  = 2;				/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555;	/* set pins A8-15 as output */
	*GPIO_PA_DOUT  = 0xFF00; 		/* Start with leds off */

	*GPIO_PC_MODEL = 0x33333333;	/* set pins C0-7 as input */
	*GPIO_PC_DOUT  = 0xFF; 			/* Enable pull-up */

	/* Interrupts */
	*GPIO_EXTIPSELL = 0x22222222;	/* Set GPIO interrupt pins */
	*GPIO_EXTIRISE  = 0xFF;			/* Interrupt on rising transition */
	*GPIO_EXTIFALL  = 0xFF;			/* Interrupt on falling transition */
	*GPIO_IEN		= 0xFF;			/* Enable interrupts */
}

uint8_t reverseBits(uint8_t bits) {
	uint8_t rbits = 0U;
	while(bits > 0) {
		rbits <<= 1;
		if (bits & 1U)
			rbits ^= 1;
		bits >>= 1;
	}
	return rbits;
}

/*
 * LEDs
 */
#define LEDS_BASE GPIO_PA_DOUT
#define LEDS_OFFSET 8U

void gpio_leds_set(led_t d, led_state_t state) {
	if (state == ON) { /* Active low */
		*LEDS_BASE &= ~(1UL << (d+LEDS_OFFSET));
	}
	else {
		*LEDS_BASE |= 1UL << (d+LEDS_OFFSET);
	}
}

void gpio_leds_write(uint8_t out) {
	//*LEDS_BASE = ~reverseBits(out) << LEDS_OFFSET;
	*LEDS_BASE = ~out << LEDS_OFFSET;
}

/* Buttons
 * TODO: Substitute hex values with more descriptive inverted bitfields
 */
#define BTN_SW1_MASK 0xFE // Left,  left
#define BTN_SW2_MASK 0xFD // Left,  up
#define BTN_SW3_MASK 0xFB // Left,  right
#define BTN_SW4_MASK 0xF7 // Left,  down
#define BTN_SW5_MASK 0xEF // Right, left
#define BTN_SW6_MASK 0xDF // Right, up
#define BTN_SW7_MASK 0xBF // Right, right
#define BTN_SW8_MASK 0x7F // Right, down
/* may not be needed */

uint8_t gpio_btn_pressed(button_t btn) {
	return ~(*GPIO_PC_DIN >> (uint8_t)(btn)) & 1U;
}

button_t gpio_btn_lookup() {
	for (button_t btn = SW1; btn <= SW8; btn++) { // Check all buttons
		if (gpio_btn_pressed(btn))
			return btn;
	}
	return -1; /* Error / no button pressed */
}

void gpio_btn_handler() {
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
	//gpio_leds_write(1<<(snd_vol-1));
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
