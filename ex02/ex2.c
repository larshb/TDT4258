#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "gpio.h"
#include "waves.h"
#include "sound.h"

/*
 * Declaration of peripheral setup functions 
 */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();
void polling();

//#include "notes.h" //<-remove

/*
 * Your code will start executing here 
 */
int main(void)
{
	/*
	 * Call the peripheral setup functions 
	 */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);

	snd_init();
	//polling();

	/*
	 * Enable interrupt handling 
	 */
	setupNVIC();
	//polling();

	/*
	 * TODO for higher energy efficiency, sleep while waiting for
	 * interrupts instead of infinite loop for busy-waiting 
	 */
	while(1); /* Ignore below code */
	uint16_t i;
	uint8_t j;
	while (1) {
		if (++i==0)
			gpio_leds_write(j++);
	}

	return 0;
}

void setupNVIC()
{
	/*
	 * TODO use the NVIC ISERx registers to enable handling of
	 * interrupt(s) remember two things are necessary for interrupt
	 * handling: - the peripheral must generate an interrupt signal - the
	 * NVIC must be configured to make the CPU handle the signal You will
	 * need TIMER1, GPIO odd and GPIO even interrupt handling for this
	 * assignment. 
	 */
	*ISER0 |=  1U << 12 |	/* Enable timer interrupts */
			   1U <<  1 |	/* Enable GPIO even interrupts */
			   1U << 11;    /* Enable GPIO odd interrupts */
}

void busy_delay(uint8_t ticks) {
	volatile uint8_t i;
	for (i = 0; i < ticks; i++);
}

void busy_sample_tick() {
	while (*TIMER1_CNT < SAMPLE_PERIOD/2); // Wait
	while (*TIMER1_CNT > SAMPLE_PERIOD/2); // Wait
}

void polling() {
	while(1) {
		/* Play tone */
		if (gpio_btn_pressed(SW6)) {
			triangle(440);
			snd_audioOut();
		}
		if (gpio_btn_pressed(SW7)) {
			sawtooth(440);
			snd_audioOut();
		}
		if (gpio_btn_pressed(SW8)) {
			square(440);
			snd_audioOut();
		}

		gpio_btn_handler();

		busy_sample_tick();
	}
}
