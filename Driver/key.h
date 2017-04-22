#ifndef _key_H_
#define _key_H_

#include <stdint.h>
#include "stm32f10x.h"

extern volatile  uint8_t gKey;

void KeyConfig(void);
uint8_t GetRow(void);
uint8_t GetCol(void);
uint8_t GetKey(void);


static void EXTI_PA2_Enable(void);
static void EXTI_PA2_Disable(void);

void EXTI_PA2_Isr(void);

#endif //_key_H_
