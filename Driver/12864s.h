#ifndef _12864s_H_
#define _12864s_H_

/*****lib**********/
#include "stm32f10x.h"
#include "delay.h"

//#define PORT GPIO_B
#define SCLK GPIO_Pin_0 //B-10
#define SDATA GPIO_Pin_1 //B-11



#define  SCLK_SET GPIO_SetBits(GPIOB,SCLK)
#define  SCLK_CLR GPIO_ResetBits(GPIOB,SCLK)

#define  SDATA_SET GPIO_SetBits(GPIOB,SDATA)
#define  SDATA_CLR GPIO_ResetBits(GPIOB,SDATA)


/*******DECLARE***/
void LcdIoInit(void);
void LcdSendByte(unsigned char byte);
void LcdWriteCmd(unsigned char cmd);
void LcdWriteData(unsigned char data);
void LcdInit(void);
void LcdAddressSet(unsigned char x,unsigned char y);
void LcdPrintString(unsigned char x,unsigned char y,unsigned char *string);
void LcdPrintChar(unsigned char x,unsigned char y,unsigned char num);
#if 0
void LcdPrintInt(unsigned char x,unsigned char y,unsigned int num);
#else
void LcdPrintInt(unsigned char x,unsigned char y,int num);
#endif
void LcdPrintTemp(unsigned char x,unsigned char y,unsigned short t);

#endif /* _12864s_H_ */
