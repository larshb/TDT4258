#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to setup the timer 
 */
void setupTimer(uint16_t period)
{
	*CMU_HFPERCLKEN0 |= 1U << 6; /* Enable timer clock */
	*TIMER1_TOP = period; /* Set period */
	*TIMER1_IEN = 1U; /* Enable interrupt */
	*TIMER1_CMD = 1U; /* Start */

}
