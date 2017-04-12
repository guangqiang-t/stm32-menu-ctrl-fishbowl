#ifndef USART_H_
#define USART_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "stdarg.h"

void USART1_Config(void);

int fputc(int ch,FILE *f);//printf

#endif
