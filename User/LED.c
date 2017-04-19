#include "LED.H"

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
