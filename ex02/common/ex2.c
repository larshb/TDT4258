#include "efm32gg.h"

#include "gpio.h"
#include "sound.h"

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();

void enableDAC();

void polling();

int main(void) {
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	#ifndef IMPROVED
	enableDAC();
	polling();
	#endif
	
	setupNVIC(); /* Enable interrupt handling */

	/* Low power */
	//*(EMU_CTRL+4) = 7; // Disable RAM
	//__asm("wfi");

	return 0;
}

void setupNVIC() {
	*ISER0 |=  1U << 12 |	/* Enable timer interrupts */
			   1U <<  1 |	/* Enable GPIO even interrupts */
			   1U << 11;    /* Enable GPIO odd interrupts */
}
