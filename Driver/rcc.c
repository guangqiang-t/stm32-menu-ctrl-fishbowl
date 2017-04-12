#include "rcc.h"

void RCC_Configuration(void)

{
   ErrorStatus HSEStartUpStatus;
   RCC_DeInit();                                            //将外设RCC寄存器重设为默认值 
   RCC_HSEConfig(RCC_HSE_ON);                               //设置外部高速晶振HSE
   HSEStartUpStatus=RCC_WaitForHSEStartUp();                //等待HSE起振
   if(HSEStartUpStatus==SUCCESS)
   {
     FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  //预取指缓存使能
     FLASH_SetLatency(FLASH_Latency_2);                     //设置代码延时值 2 延时周期
     RCC_HCLKConfig(RCC_SYSCLK_Div1);                       //设置AHB时钟（HCLK）RCC_HCLK_Div1 APB时钟=系统时钟
     RCC_PCLK2Config(RCC_HCLK_Div1);                        //设置高速AHB时钟（PCLK2）RCC_HCLK_Div1 APB时钟=HCLK  
     RCC_PCLK1Config(RCC_HCLK_Div2);                        //设置低速AHB时钟（PCLK1）RCC_HCLK_Div2 APB1时钟=HCLK/2
     RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);    //设置PLL时钟源及倍频系数
     RCC_PLLCmd(ENABLE);                                    //使能或失能PLL
     while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)       //等待指定的RCC标志位设置成功，等待PLL初始化成功
		 {
				;
		 }
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);             //设置系统时间（SYSCLK），设置PLL为系统时钟源
     while(RCC_GetSYSCLKSource()!=0x08)                     //等待PLL成功用于系统时钟的时钟源
		 {		
				;//0x00:HSI作为系统时钟//0x04:HSE作为系统时钟//0x08:PLL作为系统时钟 
		 }                                                                                                          
   }
}
