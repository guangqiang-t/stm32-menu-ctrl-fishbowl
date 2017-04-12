#ifndef LED_H_
#define LED_H_

#include "stm32f10x.h"

void LEDInit(void);
void LEDON(unsigned char index);
void LEDOFF(unsigned char index);

#endif
