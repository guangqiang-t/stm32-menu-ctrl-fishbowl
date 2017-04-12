#include "delay.h"

void DelayUs(unsigned int time)
{
	SysTick->LOAD=CPU_F*time;
	SysTick->CTRL=0x00000005;
	while(!(SysTick->CTRL&0x00010000));
	SysTick->CTRL=0x00000004;
}

void DelayMs(unsigned int time)
{
	int i;
	for(i=990;i>0;i--)
	{
		DelayUs(time);
	}
}
