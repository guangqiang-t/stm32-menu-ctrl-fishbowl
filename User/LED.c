#include "LED.H"

/**
  * @brief init i/o of the led
  * @param None
  * @param  
  * @retval None
  */

void LEDInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	//c0 c1 c2 c3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
}

/**
  * @brief led on
  * @param None
  * @param  
  * @retval None
  */
void LEDOFF(unsigned char index)
{
	switch(index)
	{
		case 0:GPIO_ResetBits(GPIOC,GPIO_Pin_0);break;
		case 1:GPIO_ResetBits(GPIOC,GPIO_Pin_1);break;
		case 2:GPIO_ResetBits(GPIOC,GPIO_Pin_2);break;
		case 3:GPIO_ResetBits(GPIOC,GPIO_Pin_3);break;
		default:break;
	}
}

/**
  * @brief led off
  * @param None
  * @param  
  * @retval None
  */
void LEDON(unsigned char index)
{
	switch(index)
	{
		case 0:GPIO_SetBits(GPIOC,GPIO_Pin_0);break;
		case 1:GPIO_SetBits(GPIOC,GPIO_Pin_1);break;
		case 2:GPIO_SetBits(GPIOC,GPIO_Pin_2);break;
		case 3:GPIO_SetBits(GPIOC,GPIO_Pin_3);break;
		default:break;
	}
}
