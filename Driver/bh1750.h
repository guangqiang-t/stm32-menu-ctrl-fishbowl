#ifndef _bsp_bh1750_H_
#define _bsp_bh1750_H_

#include <stdint.h>
#include "stm32f10x.h"

extern  uint8_t pBH1750_RAW[2];
extern uint32_t gui32BH1750Value;

void bh1750Config(void);
void bh1750Start(void);
void bh1750WriteByte(uint8_t dat);
void bh1750ReadBuff(uint8_t *pRead);
float GetLight(uint8_t *p);

#endif //_bsp_bh1750_H_
