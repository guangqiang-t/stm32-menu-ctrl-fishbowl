#include "LED.H"
#include "delay.h"
#include "stdio.h"
/**
  * @brief init i/o of the led
  * @param None
  * @param  
  * @retval None
  */

void led_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	//c13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//c13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}

/**
  * @brief led on
  * @param None
  * @param  
  * @retval None
  */
void led_off(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

/**
  * @brief led off
  * @param None
  * @param  
  * @retval None
  */
void led_on(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}


uint8_t get_high_water(void)
{
	uint16_t i=0,n=0;
	for(i=0;i<20;i++)
	{
		DelayUs(20);
		printf("%d ",GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 0)n++;
		if(n>18) 
		{
			printf("--->high water \r\n");
			return 1;
		}
	}
	return 0;
}

uint8_t get_low_water(void)
{
	uint16_t i=0,n=0;
	for(i=0;i<20;i++)
	{
		DelayUs(20);
		//printf("%d ",GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7));
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) == 0)n++;
		if(n>18) 
		{
			printf("--->low water \r\n");
			return 1;
		}
	}
	return 0;
}

