#ifndef GPIO_H__
#define GPIO_H__

#include <stdint.h>

typedef enum { ON=0, OFF=1 } led_state_t; /* (active low) */
typedef enum { D1=0, D2, D3, D4, D5, D6, D7, D8 } led_t;

void gpio_leds_set(led_t d, led_state_t state);

typedef enum { SW1, SW2, SW3, SW4, SW5, SW6, SW7, SW8 } button_t;

uint8_t gpio_btn_pressed(button_t btn);

#endif /* end of include guard: GPIO_H__ */
