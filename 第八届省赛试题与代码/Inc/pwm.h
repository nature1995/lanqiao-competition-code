#ifndef __PWM_H
#define __PWM_H

#include "pbdata.h"

extern float Pwm1Temp;        //上下行电机占空比临时变量
extern float Pwm2Temp;        //开关门电机占空比临时变量
extern float Pwm1Control;     //上下行电机占空比控制变量
extern float Pwm2Control;     //开关门电机占空比控制变量

void PWM_Configuration(void); //脉宽调制信号配置函数
void PWM_OutputControl(void); //脉宽调制信号输出控制函数
void PWM_Control(float Channel1Pulse, float Channel2Pulse);//脉宽调制信号控制函数

#endif
