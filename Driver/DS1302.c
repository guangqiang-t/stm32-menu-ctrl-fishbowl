#include "DS1302.h"
#include "stm32f10x.h"
#include "delay.h"


//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
unsigned char  READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
unsigned char  WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

//---DS1302时钟初始化2013年1月1日星期二12点00分00秒。---//
//---存储顺序是秒分时日月周年,存储格式是用BCD码---//
unsigned char TIME[7] = {0, 0x33, 0x23, 0x22, 0x04, 0x02, 0x17};

unsigned char YearString[11]={"2017-04-22\0"};
unsigned char TimeString[9]={"23-34-00\0"};


static void DS1302_RST_L(void);
static void DS1302_RST_H(void);
static void DS1302_SCLK_L(void);
static void DS1302_SCLK_H(void);
static void DS1302_DSIO_L(void);
static void DS1302_DSIO_H(void);
static uint8_t DS1302_DSIO_READ(void);
static void DS1302_DSIO_IN(void);
static void DS1302_DSIO_OUT(void);

void DS1302_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//c3c4c5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
}

static void DS1302_RST_L(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}

static void DS1302_RST_H(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
}

static void DS1302_SCLK_L(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
}

static void DS1302_SCLK_H(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
}

static void DS1302_DSIO_L(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}

static void DS1302_DSIO_H(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
}

static uint8_t DS1302_DSIO_READ(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
}

static void DS1302_DSIO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//c0 c1 c2 c3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
static void DS1302_DSIO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//c0 c1 c2 c3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
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

	DS1302_DSIO_OUT();
	
	for (n=0; n<8; n++)//开始传送八位地址命令
	{
		if(addr & 0x01)//数据从低位开始传送
		{
			DS1302_DSIO_H();
		}
		else
		{
			DS1302_DSIO_L();
		}
		addr >>= 1;
		DS1302_SCLK_H();//数据在上升沿时，DS1302读取数据
		DelayUs(1);
		DS1302_SCLK_L();
		DelayUs(1);
	}
	
	for (n=0; n<8; n++)//写入8位数据
	{
		if(dat & 0x01)//数据从低位开始传送
		{
			DS1302_DSIO_H();
		}
		else
		{
			DS1302_DSIO_L();
		}
		dat >>= 1;
		DS1302_SCLK_H();//数据在上升沿时，DS1302读取数据
		DelayUs(1);
		DS1302_SCLK_L();
		DelayUs(1);	
	}	
		 
	DS1302_RST_L();//传送数据结束
	DelayUs(1);
}

unsigned char Ds1302Read(unsigned char addr)
{
	unsigned char n=0,dat=0,dat1=0;
	DS1302_RST_L();
	DelayUs(1);

	DS1302_SCLK_L();//先将DS1302_SCLK置低电平。
	DelayUs(1);
	DS1302_RST_H();//然后将DS1302_RST(CE)置高电平。
	DelayUs(1);

	DS1302_DSIO_OUT();
	for(n=0; n<8; n++)//开始传送八位地址命令
	{
		if(addr & 0x01)//数据从低位开始传送
		{
			DS1302_DSIO_H();
		}
		else
		{
			DS1302_DSIO_L();
		}
		addr >>= 1;
		DS1302_SCLK_H();//数据在上升沿时，DS1302读取数据
		DelayUs(1);
		DS1302_SCLK_L();//DS1302下降沿时，放置数据
		DelayUs(1);
	}
	
	
	DS1302_DSIO_IN();
	DelayUs(2);
	for(n=0; n<8; n++)//读取8位数据
	{
		DelayUs(1);
		DS1302_SCLK_L();//DS1302下降沿时，放置数据
		DelayUs(1);
		DS1302_SCLK_H();
		dat1 = DS1302_DSIO_READ();//从最低位开始接收
		//dat = (dat>>1)|(dat1<<7);
		dat>>=1;
		if(dat1)dat|=0x80;
	
	}

	DS1302_RST_L();
	DelayUs(20);	//以下为DS1302复位的稳定时间,必须的。
	DS1302_DSIO_L();
	DS1302_SCLK_L();
	return dat;	
}

void Ds1302Init(unsigned char index)
{
	unsigned char n=0;
	DS1302_Config();
	if(index)
	{
		Ds1302Write(0x8E,0X00);		 //禁止写保护，就是关闭写保护功能
		for (n=0; n<7; n++)//写入7个字节的时钟信号：分秒时日月周年
		{
			Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
		}
		Ds1302Write(0x8E,0x80);		 //打开写保护功能
	}
}

void Ds1302ReadTime(void)
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



