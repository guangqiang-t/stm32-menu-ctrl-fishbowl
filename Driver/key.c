#include "key.h"
#include "delay.h"
#include "stdio.h"

volatile  uint8_t gKey=0;

void KeyConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//a2 INT0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);//k1--k6  PB10-->PB15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_0);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	#if 0
	EXTI_DeInit();
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_PA2_Enable();
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	#endif
	
}

void PortReset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);//k1--k6  PB10-->PB15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

void EXTI_PA2_Enable(void)
{
	#if 1
	EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	EXTI_DeInit();
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	#else
	EXTI->IMR = EXTI_Line2;
	#endif
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}

void EXTI_PA2_Disable(void)
{
	#if 1
	EXTI_InitTypeDef EXTI_InitStructure;
	
	EXTI_DeInit();
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
	#if 1
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//DelayUs(100);
	
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==0)tmp=1;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0)tmp=2;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)tmp=3;
	
	#endif 
	return tmp;
}

uint8_t GetCol(void)
{
	uint8_t tmp=0;
	#if 1
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	//DelayUs(100);
	

	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 0)tmp=1;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0)tmp=2;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0)tmp=3;
	#endif
	return tmp;
}

uint8_t GetKey(void)
{
	uint8_t r[2]={0};
	uint8_t c[2]={0};
	uint8_t i=0;
	uint8_t k=0;

	if(GetCol() != 0)//press
	{
		DelayMs(20);
		if(GetRow() != 0)
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
			if((r[0]==r[1]) && (c[0]==c[1]))
			{
				
				k = ((r[0]-1)*3+c[0]);
			}
			else 
			{
				k = 253;
			}
		}
		else
		{
			k = 254;
		}
	}
	else
	{
		k = 255;
	}

	while(GetCol());  // press up 
	PortReset();
	return k;
}


void EXTI_PA2_Isr(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)) 
		{
		
			EXTI_ClearFlag(EXTI_Line2);  
			EXTI_ClearITPendingBit(EXTI_Line2);
			EXTI_PA2_Disable();
			gKey=GetKey();
			printf("enter exti---------------------------INT GET a Key %d\r\n",gKey);
			EXTI_PA2_Enable();	
		} 
		else
		{
			printf("int err\r\n");
		}
}



