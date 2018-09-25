#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO()
{
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL = 2;	/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555;	/* set pins A8-15 as output */
	*GPIO_PA_DOUT = 0xFF00; /* Start with leds off */

	*GPIO_PC_MODEL = 0x33333333;	/* set pins C0-7 as input */
	*GPIO_PC_DOUT = 0xFF; /* Enable pull-up */
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

typedef enum { ON=0, OFF=1 } led_state_t; /* (active low) */
typedef enum { D1=0, D2, D3, D4, D5, D6, D7, D8 } led_t;

void gpio_leds_set(led_t d, led_state_t state) {
	if (state == ON) { /* Active low */
		*LEDS_BASE &= ~(1UL << (d+LEDS_OFFSET));
	}
	else {
		*LEDS_BASE |= 1UL << (d+LEDS_OFFSET);
	}
}

void gpio_leds_write(uint8_t out) {
	*LEDS_BASE = ~reverseBits(out) << LEDS_OFFSET;
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

typedef enum { SW1, SW2, SW3, SW4, SW5, SW6, SW7, SW8 } button_t;

uint8_t gpio_btn_pressed(button_t btn) {
	return ~(*GPIO_PC_DIN >> (uint8_t)(btn)) & 1U;
}
