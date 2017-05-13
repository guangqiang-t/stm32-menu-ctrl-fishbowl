#include "bh1750.h"
#include "IIC.h"
#include "delay.h"

#define SLAVEADDRESS 0x46
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x21
#define BH1750_RSET 0x07


/*
addr=1 1011100
addr=0 0100011
*/

uint8_t  pBH1750_RAW[2]={0};
uint32_t g_Light=0;

void bh1750Config(void)
{
	IIC_Config();
}

void bh1750Start(void)
{
	bh1750WriteByte(BH1750_ON);
	DelayUs(1);
	bh1750WriteByte(BH1750_RSET);
	DelayUs(1);
	bh1750WriteByte(BH1750_ONE);
	DelayMs(180);
}

void bh1750WriteByte(uint8_t dat)
{
	IIC_Start();
	IIC_Send_Byte(SLAVEADDRESS);
	while(IIC_Wait_Ack());
	IIC_Send_Byte(dat);  
	while(IIC_Wait_Ack());
	IIC_Stop();
}

void bh1750ReadBuff(uint8_t *pRead)
{   
	IIC_Start();
	IIC_Send_Byte(SLAVEADDRESS+1);
	while(IIC_Wait_Ack());
	
	*pRead++ = IIC_Read_Byte(1);
	*pRead = IIC_Read_Byte(0);
	
   IIC_Stop();
}

float GetLight(uint8_t *p)
{
	uint16_t tmp=0;
	tmp=((*p)*256)+(*(p+1));
//	return (100*(float)(tmp)/1.2);
	return tmp;
}

