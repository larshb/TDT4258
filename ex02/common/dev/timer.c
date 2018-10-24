#include "efm32gg.h"

void setupTimer(uint16_t period) {
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1; /* Enable timer clock */
	*TIMER1_TOP = period; /* Set period */
	*TIMER1_IEN = 1U; /* Enable interrupt */
	*TIMER1_CMD = 1U; /* Start */
}
