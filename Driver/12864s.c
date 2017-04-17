#include "12864s.h"

//unsigned char tt=0;
void LcdIoInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SCLK|SDATA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void LcdSendByte(unsigned char byte)
{
  unsigned char i;
  //SCLK_SET;
  for(i=0;i<8;i++)
  {
    if(byte&0x80)
      SDATA_SET;
    else
      SDATA_CLR;
  SCLK_SET;
  SCLK_CLR;
  byte=(byte<<1);
  }
}

void LcdWriteCmd(unsigned char cmd)
{
//串口控制格式（11111AB0）
//A数据方向控制，A=H时读，A=L时写
//B数据类型选择，B=H时为显示数据，B=L时为命令
  //lcd_busy_test();
  LcdSendByte(0xF8);
  LcdSendByte(cmd&0xF0);
  LcdSendByte((cmd<<4)&0xF0);
  DelayUs(10);
}

void LcdWriteData(unsigned char data)
{
  //lcd_busy_test();
  LcdSendByte(0xFA);
  LcdSendByte(data&0xF0);
  LcdSendByte((data<<4)&0xF0);
  DelayUs(10);
}
 
void LcdInit(void)
{
  LcdIoInit();
  DelayMs(5);
  LcdWriteCmd(0x30);
  DelayMs(2);
  LcdWriteCmd(0x0c);
  DelayMs(2);
  LcdWriteCmd(0x01);
  DelayMs(2);
  LcdWriteCmd(0x06);
  DelayMs(2);
}

void LcdAddressSet(unsigned char x,unsigned char y)
{
  unsigned char row;
  switch (x)
    {
    case 1 : row=0x80; break;
    case 2 : row=0x90; break;
    case 3 : row=0x88; break;
    case 4 : row=0x98; break;
    default : break;
    }
  LcdWriteCmd(row+y-1);
}

void LcdPrintString(unsigned char x,unsigned char y,unsigned char *string)
{
  LcdAddressSet(x,y);
  while(*string)
  {
    LcdWriteData(*string++);
  }
}

void lcdPrintChar(unsigned char x,unsigned char y,unsigned char num)
{
  LcdAddressSet(x,y);
  LcdWriteData(num/100+'0');
  LcdWriteData(num/10%10+'0');
  LcdWriteData(num%10+'0');
}

void LcdPrintInt(unsigned char x,unsigned char y,unsigned int num)
{
  unsigned char n[5]={"00000"},i;
  for(i=0;i<5;i++)
  {
    n[4-i]=num%10+'0';
    num/=10;
  }
  LcdAddressSet(x,y);
	DelayMs(1);
   for(i=0;i<5;i++)
  {
    LcdWriteData(n[i]);
   }
}


