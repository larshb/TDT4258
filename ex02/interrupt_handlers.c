#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "sound.h"

#include "melodies.h"

/*
 * TIMER1 interrupt handler 
 */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*TIMER1_IFC = 1U; /* Clear interrupt */

	/* Play startup sound */
	audioOut();
	static int done = 0;
	if (!done) {
		done = snd_PlayMelody2(&mel_mushroom2);
	}
}

/*
 * GPIO even pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
}

/*
 * GPIO odd pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
}
