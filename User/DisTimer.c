#include "DisTimer.h"


void Timer3Config(uint16_t nms)//max 260   65535/250 000=0.26214
{
	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		TIM_DeInit(TIM3); 
		TIM_TimeBaseStructure.TIM_Period = nms*250;
		
		TIM_TimeBaseStructure.TIM_Prescaler = (72000000/1000000 - 1);              
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;     
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	
  
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);					  
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
			 
		TIM_Cmd(TIM3, ENABLE); ;
}



