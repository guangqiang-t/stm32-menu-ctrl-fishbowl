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

void assert_failed(uint8_t* file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}

int main (void)
{
	led_config();
	USART1_Config();
//	LcdInit();
	menu_all_config();
	NVIC_Config();
	Timer3Config(100);
	bh1750Config();
	DelayMs(200);
	printf("sys init ok \r\n");
	KeyConfig();
	
	
	while(1)

	{
//		menu_page_up();
		menu_page_down();
		
		bh1750Start();
		bh1750ReadBuff(pBH1750_RAW);
		gui32BH1750Value=GetLight(pBH1750_RAW);
		
		printf("lig is %d\r\n ",gui32BH1750Value);
//		printf("GetKey:%d\r\n",GetKey());
		if(gui32BH1750Value<100)gui32BH1750Value=100;
		led_on();
		DelayMs(gui32BH1750Value);
		led_off();
		DelayMs(gui32BH1750Value);
//		LcdPrintInt(1,1,22);
		
	}
	
//	return 0;
}

