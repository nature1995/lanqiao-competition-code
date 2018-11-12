#ifndef __LED_H
#define __LED_H

#include "pbdata.h"

#define LED1    GPIO_Pin_8
#define LED2    GPIO_Pin_9
#define LED3    GPIO_Pin_10
#define LED4    GPIO_Pin_11
#define LED5    GPIO_Pin_12
#define LED6    GPIO_Pin_13
#define LED7    GPIO_Pin_14
#define LED8    GPIO_Pin_15
#define LEDALL	LED1 | LED2 | LED3 | LED4 | LED5 | LED6 | LED7 | LED8

extern uint8_t LedControl; //LED使能控制变量
extern uint8_t LedCycle;   //LED循环控制变量
extern uint32_t LedTiming; //LED定时控制变量

void LED_Configuration(void);                   //LED配置函数
void LED_Display(void);                         //LED显示控制函数
void LED_CycleControl(uint8_t LedControlTemp);  //LED循环控制函数
void LED_Control(uint16_t Led,uint8_t LedTemp); //LED开启关闭函数

#endif
