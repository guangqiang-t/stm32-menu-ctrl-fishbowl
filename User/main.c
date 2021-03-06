#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "USART.h"
 #include "LED.h"
#include "12864s.h"
#include "DS1302.h"
#include "menu.h"
#include "DisTimer.h"
#include "bh1750.h"
#include "IIC.h"
#include "key.h"
#include "DS18B20.h"
#include "pwm.h"
#include "relay.h"
#include "task.h"



void assert_failed(uint8_t* file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}

int main (void)
{
//	uint8_t k=0;
	
	
	DelayMs(500);
	led_config();
	USART1_Config();
	DelayMs(300);
	LcdInit();
	menu_all_config();
	NVIC_Config();
	Ds1302Init(0);
//	Timer3Config(180);
	bh1750Config();
	pwm_config(100);
	
	printf("sys init ok \r\n");
	ds18b20_start();
	KeyConfig();
	RelayConfig();
	task_init(gp_task_huanshui,stask_disable,stask_start);
	
	while(1)

	{
		led_on();
		DelayMs(80);
		led_off();
		__handle_key();
		menu_display();
		__handle_operate();
	}
	
	return 0;
}

