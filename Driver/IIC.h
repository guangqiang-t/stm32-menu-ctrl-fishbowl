#ifndef _bsp_iic_H_
#define _bsp_iic_H_

#include "stm32f10x.h"
#include <stdint.h>

void IIC_Config(void);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);


#endif//_bsp_iic_H_
