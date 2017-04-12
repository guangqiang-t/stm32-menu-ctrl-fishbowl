#include "DS1302.h"
#include <reg51.h>
#include <intrins.h>

sbit DS1302_DSIO=P2^5;
sbit DS1302_RST=P2^6;
sbit DS1302_SCLK=P2^4;

//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
unsigned char code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
unsigned char code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

//---DS1302时钟初始化2013年1月1日星期二12点00分00秒。---//
//---存储顺序是秒分时日月周年,存储格式是用BCD码---//
unsigned char TIME[7] = {0, 0, 0x12, 0x15, 0x05, 0x02, 0x16};

unsigned char YearString[11]={"2016-01-01\0"};
unsigned char TimeString[9]={"12-00-00\0"};

void DS1302_Config(void)
{
	
}

void DS1302_RST_L(void)
{
	
}

void DS1302_RST_H(void)
{
	
}

void DS1302_SCLK_L(void)
{
	
}

void DS1302_SCLK_H(void)
{
	
}

void DS1302_DSIO_L(void)
{
	
}

void DS1302_DSIO_H(void)
{
	
}

uint8_t DS1302_DSIO_READ(void)
{
	
}

void DS1302_DSIO_IN(void)
{
	
}
void DS1302_DSIO_OUT(void)
{
	
}

//-------------------end io ctrl---------------------------------------



void Ds1302Write(unsigned char addr, unsigned char dat)
{
	unsigned char n;
	DS1302_RST_L();
	DelayUs(1);

	DS1302_SCLK_L();//先将DS1302_SCLK置低电平。
	DelayUs(1);
	DS1302_RST_H();//然后将DS1302_RST(CE)置高电平。										      
	DelayUs(1);

	for (n=0; n<8; n++)//开始传送八位地址命令
	{
		DS1302_DSIO = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		DS1302_SCLK_H();//数据在上升沿时，DS1302读取数据
		DelayUs(1);
		DS1302_SCLK_L();
		DelayUs(1);
	}
	for (n=0; n<8; n++)//写入8位数据
	{
		DS1302_DSIO = dat & 0x01;
		dat >>= 1;
		DS1302_SCLK_H();//数据在上升沿时，DS1302读取数据
		DelayUs(1);
		DS1302_SCLK_L();
		DelayUs(1);	
	}	
		 
	DS1302_RST = 0;//传送数据结束
	DelayUs(1);
}

unsigned char Ds1302Read(unsigned char addr)
{
	unsigned char n,dat,dat1;
	DS1302_RST_L();
	DelayUs(1);

	DS1302_SCLK_L();//先将DS1302_SCLK置低电平。
	DelayUs(1);
	DS1302_RST_H();//然后将DS1302_RST(CE)置高电平。
	DelayUs(1);

	for(n=0; n<8; n++)//开始传送八位地址命令
	{
		DS1302_DSIO = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		DS1302_SCLK_H();//数据在上升沿时，DS1302读取数据
		DelayUs(1);
		DS1302_SCLK_L();//DS1302下降沿时，放置数据
		DelayUs(1);
	}
	DelayUs(1);
	for(n=0; n<8; n++)//读取8位数据
	{
		dat1 = DS1302_DSIO;//从最低位开始接收
		dat = (dat>>1) | (dat1<<7);
		DS1302_SCLK_H();
		DelayUs(1);
		DS1302_SCLK_L();//DS1302下降沿时，放置数据
		DelayUs(1);
	}

	DS1302_RST_L();
	DelayUs(1);	//以下为DS1302复位的稳定时间,必须的。
	DS1302_SCLK_H();
	DelayUs(1);
	DS1302_DSIO_L();
	DelayUs(1);
	DS1302_DSIO_H();
	DelayUs(1);
	return dat;	
}

void Ds1302Init()
{
	unsigned char n;
	Ds1302Write(0x8E,0X00);		 //禁止写保护，就是关闭写保护功能
	for (n=0; n<7; n++)//写入7个字节的时钟信号：分秒时日月周年
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	Ds1302Write(0x8E,0x80);		 //打开写保护功能
}

void Ds1302ReadTime()
{
	unsigned char n;
	for (n=0; n<7; n++)//读取7个字节的时钟信号：分秒时日月周年
	{
		TIME[n] = Ds1302Read(READ_RTC_ADDR[n]);
	}
		
}

void TimeConvertToString(void)
{
	TimeString[0]='0'+(TIME[2]/16);//h
	TimeString[1]='0'+(TIME[2]&0x0f);
	
	TimeString[3]='0'+(TIME[1]/16);//m
	TimeString[4]='0'+(TIME[1]&0x0f);
	
	TimeString[6]='0'+(TIME[0]/16);//s
	TimeString[7]='0'+(TIME[0]&0x0f);
	
	YearString[2]='0'+(TIME[6]/16);//y
	YearString[3]='0'+(TIME[6]&0x0f);
	
	YearString[5]='0'+(TIME[4]/16);//m
	YearString[6]='0'+(TIME[4]&0x0f);
	
	YearString[8]='0'+(TIME[3]/16);//d
	YearString[9]='0'+(TIME[3]&0x0f);
}


