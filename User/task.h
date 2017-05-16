#ifndef _task_H_
#define _task_H_

#include <stdint.h>

typedef struct 
{
	uint8_t enable;
	uint8_t state;
}task;

//extern task task1;

enum task_state{stask_enable,stask_disable,stask_start,stask_wait,stask_end,s1,s2,s3,s4,s5};
extern uint8_t g_cnt;

extern task task_huanshui;
extern task *gp_task_huanshui;

void task_init(task *t,uint8_t enable,uint8_t stat);
void task_enable(task *t);
void task_disable(task *t);
void task_wait(task *t);
void task_end(task *t);
void water_exchange(uint8_t sta);

#endif //_task_H_
