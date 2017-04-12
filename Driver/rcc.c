#include "rcc.h"

void RCC_Configuration(void)

{
   ErrorStatus HSEStartUpStatus;
   RCC_DeInit();                                            //������RCC�Ĵ�������ΪĬ��ֵ 
   RCC_HSEConfig(RCC_HSE_ON);                               //�����ⲿ���پ���HSE
   HSEStartUpStatus=RCC_WaitForHSEStartUp();                //�ȴ�HSE����
   if(HSEStartUpStatus==SUCCESS)
   {
     FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  //Ԥȡָ����ʹ��
     FLASH_SetLatency(FLASH_Latency_2);                     //���ô�����ʱֵ 2 ��ʱ����
     RCC_HCLKConfig(RCC_SYSCLK_Div1);                       //����AHBʱ�ӣ�HCLK��RCC_HCLK_Div1 APBʱ��=ϵͳʱ��
     RCC_PCLK2Config(RCC_HCLK_Div1);                        //���ø���AHBʱ�ӣ�PCLK2��RCC_HCLK_Div1 APBʱ��=HCLK  
     RCC_PCLK1Config(RCC_HCLK_Div2);                        //���õ���AHBʱ�ӣ�PCLK1��RCC_HCLK_Div2 APB1ʱ��=HCLK/2
     RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);    //����PLLʱ��Դ����Ƶϵ��
     RCC_PLLCmd(ENABLE);                                    //ʹ�ܻ�ʧ��PLL
     while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)       //�ȴ�ָ����RCC��־λ���óɹ����ȴ�PLL��ʼ���ɹ�
		 {
				;
		 }
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);             //����ϵͳʱ�䣨SYSCLK��������PLLΪϵͳʱ��Դ
     while(RCC_GetSYSCLKSource()!=0x08)                     //�ȴ�PLL�ɹ�����ϵͳʱ�ӵ�ʱ��Դ
		 {		
				;//0x00:HSI��Ϊϵͳʱ��//0x04:HSE��Ϊϵͳʱ��//0x08:PLL��Ϊϵͳʱ�� 
		 }                                                                                                          
   }
}
