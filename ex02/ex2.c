#include "efm32gg.h"

#include "gpio.h"
#include "waves.h"
#include "sound.h"

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();

void polling();

int main(void) {
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	//polling();
	
	setupNVIC(); /* Enable interrupt handling */

	/* Low power */
	*(EMU_CTRL+4) = 7; // Disable RAM
	__asm("wfi");

	return 0;
}

void setupNVIC()
{
	*ISER0 |=  1U << 12 |	/* Enable timer interrupts */
			   1U <<  1 |	/* Enable GPIO even interrupts */
			   1U << 11;    /* Enable GPIO odd interrupts */
}

void busy_sample_tick() {
	while (*TIMER1_CNT < SAMPLE_PERIOD/2); // Wait
	while (*TIMER1_CNT > SAMPLE_PERIOD/2); // Wait
}

void polling() {
	while(1) {
		busy_sample_tick();
	}
}
