#ifndef GPIO_H__
#define GPIO_H__

#include <stdint.h>

typedef enum { ON=0, OFF=1 } led_state_t; /* (active low) */
typedef enum { LED1=0, LED2, LED3, LED4, LED5, LED6, LED7, LED8 } led_t;

void gpio_leds_set(led_t d, led_state_t state);
void gpio_leds_write(uint8_t out);

typedef enum { SW1, SW2, SW3, SW4, SW5, SW6, SW7, SW8 } button_t;

uint8_t gpio_btn_pressed(button_t btn);
button_t gpio_btn_lookup(); /* Returns first (lowest numbered) button pressed */
void gpio_btn_handler();

#endif /* end of include guard: GPIO_H__ */
