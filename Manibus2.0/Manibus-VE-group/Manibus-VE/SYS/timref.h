#ifndef TIMREF_H_
#define TIMREF_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

extern unsigned int tim_count[6];
extern unsigned int tim_limit[6];
extern float encoder_get[6];
extern uint32_t tim_[6];
extern u8 use_pluse_num;

void encoder_Init(void);
	





#endif


