#ifndef _bsp_pwm_H_
#define _bsp_pwm_H_

#include <stdint.h>
#include "stm32f10x.h"

void pwm_config(uint16_t freq);//config the timer with freq:Hz
void sync_pwm(uint8_t p_pwm);


#endif //_bsp_pwm_H_
