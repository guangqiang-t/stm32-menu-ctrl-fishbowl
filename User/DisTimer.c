#include "DisTimer.h"
#include "menu.h"


void Timer3Config(uint16_t nms)//max 260   65535/250 000=0.26214
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIM3); 
	TIM_TimeBaseStructure.TIM_Period = nms*250;
	
	TIM_TimeBaseStructure.TIM_Prescaler = (72000000/250000 - 1);      //18M        
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);					  
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
			 
	TIM_Cmd(TIM3, ENABLE); ;
}

void Timer3_Isr(void)
{
		//清中断标识
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	//---------------- 中断处理  ---------------------
	{
			static uint16_t Timer3Cnt = 0;
			if(++Timer3Cnt == 5)//500ms
			{
				Timer3Cnt=0;
				//menu_display();
			}
	}
}

