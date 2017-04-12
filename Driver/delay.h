#ifndef delay_H_
#define delay_H_

#include "stm32f10x.h"

#define CPU_F 72

void DelayUs(unsigned int time);
void DelayMs(unsigned int time);

#endif
