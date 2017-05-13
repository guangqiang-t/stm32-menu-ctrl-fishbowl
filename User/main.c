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

	ChangeRelayStatus(chongyang,NO);
	
	DelayMs(100);
	while(1)

	{
		
		menu_page_down();
		
		bh1750Start();
		bh1750ReadBuff(pBH1750_RAW);
		g_Light=GetLight(pBH1750_RAW);
		
		Ds1302ReadTime();
		TimeConvertToString();
		printf("Now Date is :%s-%s\r\n",YearString,TimeString);
		
		printf("lig is %d\r\n ",g_Light);
		printf("GetKey:%d\r\n",GetKey());
		
		led_on();
		DelayMs(100);
		led_off();
		
		GetTemp();
		
		DelayMs(600);
		Ds1302ReadTime();
		TimeConvertToString();
		LcdPrintString(1,1,YearString);
		LcdPrintTemp(3,1,g_Temp[0]);
		LcdPrintTemp(3,6,g_Temp[1]);
		LcdPrintString(2,1,TimeString);
		LcdPrintInt(4,1,g_Light);
		
		if(g_Light >= 100)g_Light=100;
		sync_pwm((100-g_Light)/2);
		
	}
	
//	return 0;
}

