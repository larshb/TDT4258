#include "efm32gg.h"
#include "sound.h"
#include "gpio.h"

/*
 * TIMER1 interrupt handler 
 */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/* Clear interrupt flag */
	*TIMER1_IFC = 1U;

	snd_sampleTick();
}

void GPIO_IRQHandler(); /* Handle all buttons the same */

/*
 * GPIO even pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	GPIO_IRQHandler();
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
	GPIO_IRQHandler();
	/*
	 * TODO handle button pressed event, remember to clear pending
	 * interrupt 
	 */
}

void GPIO_IRQHandler() {
	*GPIO_IFC = *GPIO_IF; /* Clear interrupt flag */
	
	gpio_btn_handler();
}
