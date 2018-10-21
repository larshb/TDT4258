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
	gpio_btn_handler();
}

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() {
	GPIO_IRQHandler();
}

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() {
	GPIO_IRQHandler();
}