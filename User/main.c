#include "stm32f10x.h"
#include "delay.h"
#include "USART.h"

#include "LED.h"

int main (void)
{
	uint16_t ui8n=0;
	LEDInit();
	USART1_Config();
	
	while(1)
	{
		printf("this is stm32 %d \r\n",ui8n++);
		//GPIO_SetBits(GPIOC,GPIO_Pin_2);
		GPIOC->BSRR = GPIO_Pin_2;
		DelayMs(200);
		GPIO_ResetBits(GPIOC,GPIO_Pin_2);
		GPIOC->BRR = GPIO_Pin_2;
		//DelayMs(200);
	}
	
//	return 0;
}

