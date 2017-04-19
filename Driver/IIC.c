#include "IIC.h"
#include "delay.h"

//this fun config iic io ports   pd6---sck  pd2----sda

static void IIC_SCK_L(void);
static void IIC_SCK_H(void);
static void IIC_SDA_L(void);
static void IIC_SDA_H(void);
static void IIC_SDA_IN(void);
static void IIC_SDA_OUT(void);
static uint8_t IIC_READ_SDA(void);


void IIC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	//b5 addr b6 scl b7 sda 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);
}

void IIC_SCK_L(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}

void IIC_SCK_H(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
}
void IIC_SDA_L(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
}

void IIC_SDA_H(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
}
void IIC_SDA_IN(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	// b7 sda 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}
void IIC_SDA_OUT(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	// b7 sda 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}

uint8_t IIC_READ_SDA(void)
{
	
	uint8_t tmp;
	IIC_SDA_IN();
	tmp=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
	if(tmp!=0)
		return 1;
	else
		return 0;
}

//产生IIC起始信号
void IIC_Start(void)
{
	IIC_SDA_OUT();     //sda线输出
	IIC_SDA_H();	  	  
	IIC_SCK_H();
	DelayUs(5);
 	IIC_SDA_L();//START:when CLK is high,DATA change form high to low 
	DelayUs(5);
	IIC_SCK_L();//钳住I2C总线，准备发送或接收数据 
	DelayUs(5);
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	IIC_SDA_OUT();//sda线输出
	IIC_SCK_L();
	IIC_SDA_L();//STOP:when CLK is high DATA change form low to high
 	DelayUs(5);
	IIC_SCK_H(); 
	IIC_SDA_H();//发送I2C总线结束信号
	DelayUs(5);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint16_t ucErrTime=0;//uint8_t ucErrTime=0;
//	IIC_SDA_IN();      //SDA设置为输入  
	IIC_SDA_H();DelayUs(2);//DelayUs(1);	   
	IIC_SCK_H();DelayUs(2);//DelayUs(1);	 
	while(IIC_READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)//if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCK_L();//时钟输出0 	
	return 0;  
} 

//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCK_L();
	IIC_SDA_OUT();
	IIC_SDA_L();
	DelayUs(2);//DelayUs(2);
	IIC_SCK_H();
	DelayUs(2);//DelayUs(2);
	IIC_SCK_L();
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCK_L();
	IIC_SDA_OUT();
	IIC_SDA_H();
	DelayUs(2);//DelayUs(2);
	IIC_SCK_H();
	DelayUs(2);//DelayUs(2);
	IIC_SCK_L();
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
		IIC_SDA_OUT(); 	    
    IIC_SCK_L();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
			if(txd&0x80)
				{
					IIC_SDA_H();
				}
			else
				{
					IIC_SDA_L();
				}
       txd<<=1; 	  
		DelayUs(2);//DelayUs(2);   
		IIC_SCK_H();
		DelayUs(2);//DelayUs(2); 
		IIC_SCK_L();	
		DelayUs(2);//DelayUs(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
		{
     IIC_SCK_L(); 
     DelayUs(2);//DelayUs(2);
		IIC_SCK_H();
    receive<<=1;
     if(IIC_READ_SDA())receive++;   
		DelayUs(2);//DelayUs(2);//DelayUs(1); 
    }		
		DelayUs(8);
    if (ack)
    IIC_Ack();//发送ACK
    else
    IIC_NAck(); //发送nACK   
    return receive;
}



