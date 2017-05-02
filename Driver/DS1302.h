#ifndef __DS1302_H_
#define __DS1302_H_


extern unsigned char TIME[7];	//加入全局变量
extern unsigned char YearString[];
extern unsigned char TimeString[];

void DS1302_Config(void);
void Ds1302Write(unsigned char addr, unsigned char dat);
unsigned char Ds1302Read(unsigned char addr);
void Ds1302Init(unsigned char index);
void Ds1302ReadTime(void);
void TimeConvertToString(void);


#endif
