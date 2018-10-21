#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "sound.h"

#include "melodies.h"

#include "gpio.h" //testing
#include "waves.h"

/*
 * TIMER1 interrupt handler 
 */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/* Output sample at start of interrupt to ensure smooth playback */
	snd_audioOut();

	/* Clear interrupt flag */
	*TIMER1_IFC = 1U;

	/* Play startup sound */
	static uint8_t done = 0;
	if (!done) {
		done = snd_PlayMelody2(&mel_1up2);
	}

	/* Do as little as possible inside this ISR to make sure no
	 * samples are missed or corrupted.
	 */
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
