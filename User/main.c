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



void assert_failed(uint8_t* file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}

int main (void)
{
//	uint8_t k=0;
	led_config();
	USART1_Config();
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

	ChangeRelayStatus(jiawen,NO);
	
	DelayMs(100);
	while(1)

	{
		

		led_on();
		DelayMs(100);
		led_off();
		
		__handle_key();
		menu_display();
		
		if(g_Light >= 100)g_Light=100;
		sync_pwm((100-g_Light)/2);
		
	}
	
//	return 0;
}

