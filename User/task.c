#include "task.h"
#include "stm32f10x.h"
#include "led.h"
#include "relay.h"
#include <stdio.h>

//task task1;

uint8_t g_cnt=0;

task task_huanshui={stask_enable,stask_start};
task *gp_task_huanshui=&task_huanshui;

void task_init(task *t,uint8_t enable,uint8_t stat)
{
	t->enable = enable;
	t->state = stat;
}

void task_enable(task *t)
{
	t->enable=1;
}

void task_wait(task *t)
{
	t->state = stask_wait;
}

void task_disable(task *t)
{
	t->enable=stask_disable;
}

void task_end(task *t)
{
	t->state = stask_end;
	t->enable = stask_disable;
}


void water_exchange_config(void)
{
	task_init(gp_task_huanshui,stask_enable,stask_start);
}

void water_exchange(uint8_t sta)
{
//	uint8_t a,b,c;

	if((gp_task_huanshui->enable == stask_disable)&&(g_cnt == 1))
	{
		//gp_task_huanshui->enable =stask_enable;
		//gp_task_huanshui->state = sta;
		water_exchange_config();
		printf("output lock disable\r\n");
		g_cnt= 0;
	}
	
	if(gp_task_huanshui->enable ==stask_enable)
	{
		if(gp_task_huanshui->state == s3)//s3
		{
			ChangeRelayStatus(huanshui,NC);
			gp_task_huanshui->state = stask_end;
			gp_task_huanshui->enable = stask_disable;
			printf("\r\n\r\n------------------------>huanshui ok...\r\n");
		}

		if(gp_task_huanshui->state == s2)//s2
		{
			if(get_high_water())
			{
				gp_task_huanshui->state =s3;
				printf("\r\n\r\n--------------------------->jiashui ok...\r\n");
			}
		}
		
		if(gp_task_huanshui->state == s1)//s1
		{
			ChangeRelayStatus(paishui,NC);//关闭排水
			ChangeRelayStatus(huanshui,NO);
			gp_task_huanshui->state = s2;
			printf("\r\n\r\n-------------------------------->jiashui...\r\n");
		}
		
		
		if(gp_task_huanshui->state == stask_start)//start
		{
			printf("-----------------------------------------paishui....\r\n");
			ChangeRelayStatus(paishui,NO);//打开排水
			if(get_low_water())
			{
				gp_task_huanshui->state =s1;
				printf("\r\n\r\n----------------------------->paishui ok...\r\n");
			}
		}	
	}
	
	
}


