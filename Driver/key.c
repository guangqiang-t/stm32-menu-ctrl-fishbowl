#include "key.h"
#include "delay.h"

void KeyConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//a2 INT0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);//k1--k6  PB10-->PB15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_0);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	EXTI_DeInit();
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//	EXTI_PB2_Enable();
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void PortReset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);//k1--k6  PB10-->PB15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
}

void EXTI_PB2_Enable(void)
{
	#if 0
	EXTI_InitTypeDef EXTI_InitStructure;
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
#include <stm32f10x.h>
	EXTI_Init(&EXTI_InitStructure);
	#else
	EXTI->IMR = EXTI_Line2;
	#endif
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}

void EXTI_PB2_Disable(void)
{
	#if 0
	EXTI_InitTypeDef EXTI_InitStructure;
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
	#else
	EXTI->IMR = 0;
	#endif
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}

uint8_t GetRow(void)
{
	uint8_t tmp=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==0)tmp=1;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0)tmp=2;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)tmp=3;
	return tmp;
}

uint8_t GetCol(void)
{
	uint8_t tmp=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	

	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0)tmp=1;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)tmp=2;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==0)tmp=3;
	return tmp;
}

uint8_t GetKey(void)
{
	uint8_t r[2]={0};
	uint8_t c[2]={0};
	uint8_t i=0;

	if(GetCol())//press
	{
		DelayMs(5);
		if(GetRow())
		{
			for(i=0;i<2;i++)
			{
				r[i]=GetRow();
				DelayUs(20);
			}
			for(i=0;i<2;i++)
			{
				c[i]=GetCol();
				DelayUs(20);
			}
			if(r[0]==r[1] && c[0]==c[1])
			{
				PortReset();
				return ((c[0]-1)*3+r[0]);
			}
			else 
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	
}


void EXTIB2_Isr(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)) 
		{
			
			
			EXTI_ClearFlag(EXTI_Line2);  
			EXTI_ClearITPendingBit(EXTI_Line2);
			EXTI_PB2_Disable();
			printf("IntGetKey:%d\r\n",GetKey());
			EXTI_PB2_Enable();
//			GetKey();
			
		} 
}



