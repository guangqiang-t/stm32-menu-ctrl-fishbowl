#include "relay.h"
#include "stm32f10x.h"
#include <stdio.h>

#define USE_USART1

void RelayConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;//|GPIO_Pin_9|GPIO_Pin_10;	//b5 addr b6 scl b7 sda 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12);	
}

void ChangeRelayStatus(ClassRelay index,RelayStatus s)
{
	printf("enter relay ststus change\r\n");
	switch(index)
	{
		case (jiawen):
		{
			if(s == NO)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_8);
			}
			break;
		}
		#if 1
		case (huanshui):
		{
			if(s)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_9);
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_9);
			}
			break;
		}
		
		case (shuixunhuan):
		{
			if(s)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_10);
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_10);
			}
			break;
		}
		
		case (chongyang):
		{
			if(s)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_11);
			}
			break;
		}
		
		#endif 
		default:break;
	}
}
