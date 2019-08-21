#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"

// 72M / (719 + 1) = 0.1M �ʼ�һ������ʱ��Ϊ10us  ��ʱ���ڣ���1999 + 1�� * 10us = 20ms
#define SERVO_TIM_ARR  1999
#define SERVO_TIM_PSC  719



void servo_init(void);
void servo_angle(uint16_t angle);
void servo_debug(void);


#endif /* __SERVO_H */



