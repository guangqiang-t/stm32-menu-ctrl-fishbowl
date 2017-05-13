#include "pwm.h"

static uint16_t pwm_freq=0;

void pwm_config(uint16_t freq)//config the timer with freq:Hz
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_DeInit(TIM2); //timer 2
	pwm_freq=(uint16_t)(1000000/freq);
	TIM_TimeBaseStructure.TIM_Period = pwm_freq;	
	TIM_TimeBaseStructure.TIM_Prescaler = (72000000/1000000 - 1);                       
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);	
		
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	             
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);//ch1
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_Cmd(TIM2, ENABLE);
	
}

void sync_pwm(uint8_t p_pwm)
{
	uint16_t pwm;
	if(p_pwm > 100)
		return;
	pwm=p_pwm*pwm_freq/100;
	TIM_SetCompare2(TIM2,pwm);
}

