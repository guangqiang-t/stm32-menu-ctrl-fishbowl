

#include "stm32f10x.h"
#include "DS18B20.h"
#include "delay.h"
#include "stdio.h"
 

#define EnableINT()  
#define DisableINT() 

#define DS_PORT   		GPIOB
#define DS_DQIO   		GPIO_Pin_9
#define DS_RCC_PORT  	RCC_APB2Periph_GPIOB


#define DS_PRECISION 			0x7f   //精度配置寄存器 1f=9位; 3f=10位; 5f=11位; 7f=12位;
#define DS_AlarmTH  			0x64
#define DS_AlarmTL  			0x8a
#define DS_CONVERT_TICK 	1000

#define ResetDQ() GPIO_ResetBits(DS_PORT,DS_DQIO)
#define SetDQ()  GPIO_SetBits(DS_PORT,DS_DQIO)
#define GetDQ()  GPIO_ReadInputDataBit(DS_PORT,DS_DQIO)
 
#define USE_SERIES
 
static unsigned char TempX_TAB[16]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09};
const  unsigned char Series[2][8]={{0x28,0xFf,0x36,0xA9,0x8A,0x16,0x03,0xC6},{0x28,0xFF,0x78,0xAF,0xA1,0x16,0x04,0x52}};
unsigned short g_Temp[2]={0,0};
unsigned short g_TempMin=0,g_TempMax=0;

void ds18b20test(void)
{
	SetDQ();
	DelayUs(50);
	ResetDQ();
	DelayUs(10);
	SetDQ();
	DelayUs(50);
}

unsigned char ResetDS18B20(void)
{
 unsigned char resport;
 SetDQ();
 DelayUs(50);
 
 ResetDQ();
 DelayUs(500);  //500us （该时间的时间范围可以从480到960微秒）
 SetDQ();
 DelayUs(40);  //40us
 //resport = GetDQ();
 while(GetDQ());
 DelayUs(500);  //500us
 SetDQ();
 return resport;
}

void DS18B20WriteByte(unsigned char Dat)
{
 unsigned char i;
 for(i=8;i>0;i--)
 {
   ResetDQ();     //在15u内送数到数据线上，DS18B20在15-60u读数
  DelayUs(5);    //5us
  if(Dat & 0x01)
   SetDQ();
  else
   ResetDQ();
  DelayUs(65);    //65us
  SetDQ();
  DelayUs(2);    //连续两位间应大于1us
  Dat >>= 1; 
 } 
}


unsigned char DS18B20ReadByte(void)
{
 unsigned char i,Dat;
 SetDQ();
 DelayUs(5);
 for(i=8;i>0;i--)
 {
   Dat >>= 1;
    ResetDQ();     //从读时序开始到采样信号线必须在15u内，且采样尽量安排在15u的最后
  DelayUs(5);   //5us
  SetDQ();
  DelayUs(5);   //5us
  if(GetDQ())
    Dat|=0x80;
  else
   Dat&=0x7f;  
  DelayUs(65);   //65us
  SetDQ();
 }
 return Dat;
}


void ReadRom(unsigned char *Read_Addr)
{
 unsigned char i;

 DS18B20WriteByte(ReadROM);
  
 for(i=8;i>0;i--)
 {
  *Read_Addr=DS18B20ReadByte();
  Read_Addr++;
 }
}


void DS18B20Init(unsigned char Precision,unsigned char AlarmTH,unsigned char AlarmTL)
{
 DisableINT();
 ResetDS18B20();
 //ReadRom((unsigned char *)Series[0]);

 DS18B20WriteByte(SkipROM); 
 DS18B20WriteByte(WriteScratchpad);
 DS18B20WriteByte(AlarmTL);
 DS18B20WriteByte(AlarmTH);
 DS18B20WriteByte(Precision);

 ResetDS18B20();
 DS18B20WriteByte(SkipROM); 
 DS18B20WriteByte(CopyScratchpad);
 EnableINT();

 while(!GetDQ());  //等待复制完成 ///////////
}


void DS18B20StartConvert(void)
{
 DisableINT();
 ResetDS18B20();
 DS18B20WriteByte(SkipROM); 
 DS18B20WriteByte(StartConvert); 
 EnableINT();
}

void DS18B20_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 
 RCC_APB2PeriphClockCmd(DS_RCC_PORT, ENABLE);

 GPIO_InitStructure.GPIO_Pin = DS_DQIO;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //2M时钟速度
 GPIO_Init(DS_PORT, &GPIO_InitStructure);
}


void ds18b20_start(void)
{
 DS18B20_Configuration();
 DS18B20Init(DS_PRECISION, DS_AlarmTH, DS_AlarmTL);
 DS18B20StartConvert();
 PrintSeries((unsigned char *)Series[0]);
}


unsigned short ds18b20_read(unsigned char index)
{
 unsigned char TemperatureL,TemperatureH;
 unsigned int  Temperature;

 DisableINT();
 ResetDS18B20();
 #ifdef USE_SERIES
	DS18B20WriteByte(MatchROM);
	DS18B20SendSeries((unsigned char *)Series[index]);
 #else
	 DS18B20WriteByte(SkipROM); 
 #endif
 DS18B20WriteByte(ReadScratchpad);
 TemperatureL=DS18B20ReadByte();
 TemperatureH=DS18B20ReadByte(); 
 ResetDS18B20();
 EnableINT();

 if(TemperatureH & 0x80)
  {
  TemperatureH=(~TemperatureH) | 0x08;
  TemperatureL=~TemperatureL+1;
  if(TemperatureL==0)
   TemperatureH+=1;
  }

 TemperatureH=(TemperatureH<<4)+((TemperatureL&0xf0)>>4);
 TemperatureL=TempX_TAB[TemperatureL&0x0f];

 //bit0-bit7为小数位，bit8-bit14为整数位，bit15为正负位
 Temperature=TemperatureH;
 Temperature=(Temperature<<8) | TemperatureL; 

 DS18B20StartConvert();

 return  Temperature;

}

void PrintSeries(unsigned char *p)
{
	unsigned char i=0;
	printf("Series is \r\n");
	for(i=0;i<8;i++)
	{
		printf("#%x",*p++);
	}
	printf("\r\n");
}

void DS18B20SendSeries(unsigned char *addr)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		DS18B20WriteByte(*addr++);
	}
}

void GetTemp(void)
{
	g_Temp[0]=(ds18b20_read(0)-2800)/17;
	g_Temp[1]=(ds18b20_read(1)-2800)/17;
	if(g_Temp[0] > g_Temp[0] )
	{
		g_TempMax=g_Temp[0];
		g_TempMin=g_Temp[1];
	}
	else
	{
		g_TempMax=g_Temp[1];
		g_TempMin=g_Temp[0];
	}
	
}

