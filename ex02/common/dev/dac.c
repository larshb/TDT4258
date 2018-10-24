#include "efm32gg.h"

void enableDAC();

void setupDAC() {
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0; /* enable DAC clock */
	/* NOTE DAC clock prescaler seems to have little effect */
	//*DAC0_CTRL = 0x50000; /* Prescale DAC clock */
	*DAC0_CTRL = 1U << 4; /* Enable DAC output pin, disable (ADC/ACMP) */
	//*DAC0_CTRL |= 1 << 0; /* Differential mode */
	//*DAC0_CTRL |= 1 << 1; /* Sine mode */
	//*DAC0_CTRL |= 1 << 2; /* Sample-hold */

	enableDAC();
}

void enableDAC() {
	*DAC0_CH0CTRL = 1U; /* Enable left channel (automatic refresh) */
	*DAC0_CH1CTRL = 1U; /* Enable right channel (automatic refresh)  */
}

void disableDAC() {
	#ifndef IMPROVED
	return;
	#endif
    *DAC0_CH0CTRL = 0U;
    *DAC0_CH1CTRL = 0U;
}
