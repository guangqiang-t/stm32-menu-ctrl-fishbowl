#include "delay.h"

void DelayUs(unsigned int time)
{
	#if 0
	SysTick->LOAD=CPU_F*time;
	SysTick->CTRL=0x00000005;
	while(!(SysTick->CTRL&0x00010000));
	SysTick->CTRL=0x00000004;
	#else 
	  uint32_t temp;
    SysTick->LOAD=9*time;
    SysTick->CTRL=0X01;
    SysTick->VAL=0;
    do
    {
        temp=SysTick->CTRL;
    }
    while((temp&0x01)&&(!(temp&(1<<16))));
    SysTick->CTRL=0;
    SysTick->VAL=0;
	#endif
}

void DelayMs(unsigned int time)
{
	int i;
	for(i=995;i>0;i--)
	{
		DelayUs(time);
	}
}
