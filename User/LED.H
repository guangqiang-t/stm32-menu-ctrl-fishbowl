#ifndef LED_H_
#define LED_H_

#include "stm32f10x.h"
#include <stdint.h>

void led_config(void);
void led_on(void);
void led_off(void);
uint8_t get_high_water(void);
uint8_t get_low_water(void);

#endif
