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

button_t gpio_btn_lookup() {
	for (button_t btn = SW1; btn <= SW8; btn++) { // Check all buttons
		if (gpio_btn_pressed(btn))
			return btn;
	}
	return -1; /* Error / no button pressed */
}


/* Buttons
 * TODO: Substitute hex values with more descriptive inverted bitfields
 */
#define BTN_SW1_MASK 0xFE // Left,  left
#define BTN_SW2_MASK 0xFD // Left,  up
#define BTN_SW3_MASK 0xFB // Left,  right
#define BTN_SW4_MASK 0xF7 // Left,  down
#define BTN_SW5_MASK 0xEF // Right, left
#define BTN_SW6_MASK 0xDF // Right, up
#define BTN_SW7_MASK 0xBF // Right, right
#define BTN_SW8_MASK 0x7F // Right, down
/* may not be needed */

uint8_t reverseBits(uint8_t bits) {
	uint8_t rbits = 0U;
	while(bits > 0) {
		rbits <<= 1;
		if (bits & 1U)
			rbits ^= 1;
		bits >>= 1;
	}
	return rbits;
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

#include "mario.c"

typedef struct {
	uint8_t x;
	uint8_t n;
	uint16_t* y;
} waveform_t;

void playSample(waveform_t* wav) {
	snd_out = wav->y[wav->x];
	wav->x = (wav->x + 1) % wav->n;
	snd_audioOut();
}

uint16_t sine480hz_y[100] = {2048, 2176, 2304, 2431, 2557, 2680, 2801, 2919, 3034, 3145, 3251, 3353, 3449, 3540, 3625, 3704, 3776, 3842, 3900, 3951, 3995, 4031, 4059, 4079, 4091, 4095, 4091, 4079, 4059, 4031, 3995, 3951, 3900, 3842, 3776, 3704, 3625, 3540, 3449, 3353, 3251, 3145, 3034, 2919, 2801, 2680, 2557, 2431, 2304, 2176, 2048, 1919, 1791, 1664, 1538, 1415, 1294, 1176, 1061, 950, 844, 742, 646, 555, 470, 391, 319, 253, 195, 144, 100, 64, 36, 16, 4, 0, 4, 16, 36, 64, 100, 144, 195, 253, 319, 391, 470, 555, 646, 742, 844, 950, 1061, 1176, 1294, 1415, 1538, 1664, 1791, 1919};
waveform_t sine480hz = {
	.y = sine480hz_y,
	.x = 0,
	.n = 100
};

waveform_t fireball = {
	.y = fireball_y,
	.x = 0,
	.n = 9672
};

uint16_t random_y[9] = {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000};//, 3000, 2000, 1000};
waveform_t random = {
	.y = random_y,
	.x = 0,
	.n = 9
};

void sawtooth_simple() {
	unsigned int factor = 1;
	snd_out=(snd_out+factor)%snd_vol;
	*DAC0_CH0DATA = snd_out;
	*DAC0_CH1DATA = snd_vol-snd_out;
	busy_sample_tick();
}

void square_simple(){
	static int i = 0;
	i=(i+1)%100;
	snd_out = i<50?0:SND_VOL_MAX;
}