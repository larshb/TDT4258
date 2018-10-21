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
	uint16_t i;
	uint8_t j;
	while (1) {
		i++;
		if (i==0) {
			gpio_leds_write(j++);
		}
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
	*ISER0 |= 1U << 12; /* Enable timer interrupts */
}

/*
 * if other interrupt handlers are needed, use the following names:
 * NMI_Handler HardFault_Handler MemManage_Handler BusFault_Handler
 * UsageFault_Handler Reserved7_Handler Reserved8_Handler
 * Reserved9_Handler Reserved10_Handler SVC_Handler DebugMon_Handler
 * Reserved13_Handler PendSV_Handler SysTick_Handler DMA_IRQHandler
 * GPIO_EVEN_IRQHandler TIMER0_IRQHandler USART0_RX_IRQHandler
 * USART0_TX_IRQHandler USB_IRQHandler ACMP0_IRQHandler ADC0_IRQHandler
 * DAC0_IRQHandler I2C0_IRQHandler I2C1_IRQHandler GPIO_ODD_IRQHandler
 * TIMER1_IRQHandler TIMER2_IRQHandler TIMER3_IRQHandler
 * USART1_RX_IRQHandler USART1_TX_IRQHandler LESENSE_IRQHandler
 * USART2_RX_IRQHandler USART2_TX_IRQHandler UART0_RX_IRQHandler
 * UART0_TX_IRQHandler UART1_RX_IRQHandler UART1_TX_IRQHandler
 * LEUART0_IRQHandler LEUART1_IRQHandler LETIMER0_IRQHandler
 * PCNT0_IRQHandler PCNT1_IRQHandler PCNT2_IRQHandler RTC_IRQHandler
 * BURTC_IRQHandler CMU_IRQHandler VCMP_IRQHandler LCD_IRQHandler
 * MSC_IRQHandler AES_IRQHandler EBI_IRQHandler EMU_IRQHandler 
 */

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

		/* Volume control */
		if (gpio_btn_pressed(SW2) && snd_vol<SND_VOL_MAX) {
			snd_volAdjust(UP);
			while(gpio_btn_pressed(SW2)); /* Debounce */
		}
		if (gpio_btn_pressed(SW4) && snd_vol>0){
			snd_volAdjust(DOWN);
			while(gpio_btn_pressed(SW4)); /* Debounce */
		}

		busy_sample_tick();
	}
}
