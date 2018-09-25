#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * The period between sound samples, in clock cycles 
 */
#define	  FUDGE_FACTOR 964/880
#define   SAMPLE_PERIOD (uint16_t)(292*FUDGE_FACTOR) //292 /* ~ 48 kHz (ticks / 14MHz [default]) */

/*
 * Declaration of peripheral setup functions 
 */
void setupGPIO();
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();
void polling();

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

	polling();

	/*
	 * Enable interrupt handling 
	 */
	setupNVIC();

	/*
	 * TODO for higher energy efficiency, sleep while waiting for
	 * interrupts instead of infinite loop for busy-waiting 
	 */
	while (1) ;

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

/* TODO: Cleanup */

void busy_delay(uint8_t ticks) {
	volatile uint8_t i;
	for (i = 0; i < ticks; i++);
}

void busy_sample_tick() {
	while (*TIMER1_CNT < SAMPLE_PERIOD/2); // Wait
	//while (*TIMER1_CNT > SAMPLE_PERIOD/2); // Wait
}

#define SND_OUT_MAX 0xFFF
#define SND_VOL_MAX 100
volatile uint32_t snd_out;
volatile uint32_t snd_vol = 50; //

typedef enum {UP, DOWN} snd_vol_adj_t;

void snd_volAdjust(snd_vol_adj_t adj) {
	if (adj == UP) {
		snd_vol = (snd_vol+1) % SND_VOL_MAX;
	}
	else
		snd_vol = snd_vol == 0 ? 0 : (snd_vol - 1);
}

void triangleForever(){
	volatile uint16_t i;
	uint16_t factor = 10;
	for (i = 0; i < 0xFFF; i+=factor){
		*DAC0_CH0DATA = i;
		*DAC0_CH1DATA = i;
	}
	for (i = 0xFFE; i > 0U; i-=factor){
		*DAC0_CH0DATA = i;
		*DAC0_CH1DATA = i;
	}
}

void sawtooth_simple() {
	unsigned int factor = 1;
	snd_out=(snd_out+factor)%snd_vol;
	*DAC0_CH0DATA = snd_out;
	*DAC0_CH1DATA = snd_vol-snd_out;
	busy_sample_tick();
}

void sawtooth(uint16_t frequency) {
	uint32_t ticks_per_wave = 48000/frequency;
	snd_out=(snd_out+SND_OUT_MAX/ticks_per_wave);
}

uint8_t triangle_up_down_toggle = 0;
uint8_t triangle_counter = 0;
void triangle(uint16_t frequency) {
	uint32_t ticks_per_wave = 48000/frequency;
	if (triangle_counter++ == ticks_per_wave/2) {
		triangle_counter = 0;
		triangle_up_down_toggle ^= 1;
	}
	if (triangle_up_down_toggle)
		snd_out=(snd_out+SND_OUT_MAX/ticks_per_wave*2);
	else
		snd_out=(snd_out-SND_OUT_MAX/ticks_per_wave*2);
}

void square_simple(){
	static int i = 0;
	i=(i+1)%100;
	snd_out = i<50?0:SND_VOL_MAX;
}

void audioOut() {
	if (snd_out > SND_OUT_MAX)
		snd_out = 0;
	//uint32_t amp = (uint32_t)snd_out*snd_vol/100;
	uint16_t amp = snd_out;
	*DAC0_CH0DATA = amp;
	*DAC0_CH1DATA = amp;
}

typedef struct {
	uint8_t x;
	uint8_t n;
	uint16_t* y;
} waveform_t;

void playSample(waveform_t* wav) {
	snd_out = wav->y[wav->x];
	wav->x = (wav->x + 1) % wav->n;
	audioOut();
}

#include "mario.c"

void polling() {
	//gpio_leds_write(0b10001100);
	//uint16_t vol = 0 * 0xFFF / 100;
	//uint8_t delay = 50;
	//int i = 0;
	//led_t d = D1;

	uint16_t sine480hz_y[100] = {2048, 2176, 2304, 2431, 2557, 2680, 2801, 2919, 3034, 3145, 3251, 3353, 3449, 3540, 3625, 3704, 3776, 3842, 3900, 3951, 3995, 4031, 4059, 4079, 4091, 4095, 4091, 4079, 4059, 4031, 3995, 3951, 3900, 3842, 3776, 3704, 3625, 3540, 3449, 3353, 3251, 3145, 3034, 2919, 2801, 2680, 2557, 2431, 2304, 2176, 2048, 1919, 1791, 1664, 1538, 1415, 1294, 1176, 1061, 950, 844, 742, 646, 555, 470, 391, 319, 253, 195, 144, 100, 64, 36, 16, 4, 0, 4, 16, 36, 64, 100, 144, 195, 253, 319, 391, 470, 555, 646, 742, 844, 950, 1061, 1176, 1294, 1415, 1538, 1664, 1791, 1919};
	waveform_t sine480hz = {
		.y = sine480hz_y,
		.x = 0,
		.n = 100
	};

	waveform_t mushroom = {
		.y = (uint16_t*)mushroom_y,
		.x = 0,
		.n = 44159
	};

	while(1) {

		//*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
		//sawtooth_simple();

		/* Busy sampling */
		
		//gpio_leds_set(d++, ON);
		/*
		if (i++ == 48000) {
			i = 0;
			gpio_leds_set(d++, ON);
		}
		*/
		//*DAC0_CH0DATA = (*DAC0_CH0DATA+10)%0xFF;
		//if (*GPIO_PC_DIN == SW5_MASK)
		/*if (gpio_btn_pressed(SW1)){
			triangle(440);
			audioOut();
		}
		if (gpio_btn_pressed(SW3)){
			square_simple();
			audioOut();
		}
		if (gpio_btn_pressed(SW2))
			snd_volAdjust(UP);
		if (gpio_btn_pressed(SW4))
			snd_volAdjust(UP);
		if (gpio_btn_pressed(SW8))*/
			playSample(&mushroom);
			playSample(&sine480hz);
		/*
		if (*GPIO_PC_DIN == 0xFF)
			snd_out = 0;
		*/
		busy_sample_tick();
	}
}