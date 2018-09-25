#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0; /* enable DAC clock */
	*DAC0_CTRL = 0x50010; /* Prescale DAC clock */
	*DAC0_CH0CTRL = 1U; /* Enable left channel */
	*DAC0_CH1CTRL = 1U; /* Enable right channel */
}
