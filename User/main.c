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

void assert_failed(uint8_t* file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}

int main (void)
{
	int tmp=0;
	
	led_config();
	USART1_Config();
	LcdInit();
	menu_all_config();
	NVIC_Config();
	Ds1302Init(0);
//	Timer3Config(180);
	bh1750Config();
	ds18b20_start();
	printf("sys init ok \r\n");
	KeyConfig();
	
	DelayMs(100);
	while(1)

	{
//		menu_page_up();
		menu_page_down();
		
		bh1750Start();
		bh1750ReadBuff(pBH1750_RAW);
		gui32BH1750Value=GetLight(pBH1750_RAW);
		
		Ds1302ReadTime();
		TimeConvertToString();
		printf("Now Date is :%s-%s\r\n",YearString,TimeString);
		
		printf("lig is %d\r\n ",gui32BH1750Value);
		printf("GetKey:%d\r\n",GetKey());
		
		//if(gui32BH1750Value<50)gui32BH1750Value=100;
		led_on();
		DelayMs(300);
		led_off();
		
		tmp=ds18b20_read();
		// ds18b20test();
		
		DelayMs(600);
		Ds1302ReadTime();
		TimeConvertToString();
		LcdPrintString(1,1,YearString);
		LcdPrintString(2,1,TimeString);
		LcdPrintInt(3,1,gui32BH1750Value);
		LcdPrintInt(4,1,tmp);
		
		
	}
	
//	return 0;
}

