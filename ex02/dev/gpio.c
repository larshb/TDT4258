#include "gpio.h"
#include "efm32gg.h"

void setupGPIO() {
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

/* LEDs */

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
	*LEDS_BASE = ~out << LEDS_OFFSET;
}

/* Buttons */

uint8_t gpio_btn_pressed(button_t btn) {
	return ~(*GPIO_PC_DIN >> (uint8_t)(btn)) & 1U;
}
