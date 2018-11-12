#ifndef __PBDATA_H
#define __PBDATA_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_tim.h"
#include "stdio.h"

#include "stm32f10x_it.h"
#include "lcd.h"
#include "led.h"
#include "key.h"
#include "rtc.h"
#include "pwm.h"

extern u32 TimingDelay;              //延时变量
extern uint32_t TimeDisplayControl;  //LCD显示控制变量
extern uint32_t PlatformMove;        //平台移动时间变量
extern uint32_t PlatformTiming;      //平台停留时间变量
extern uint32_t PlatformTwinkle;     //平台闪烁时间变量
extern uint8_t TwinkleTemp;          //平台闪烁临时变量
extern uint8_t TwinkleControl;       //平台闪烁使能变量
extern uint8_t PlatformControl;      //楼层实时显示变量
extern uint8_t PlatformTemp;		  	 //楼层临时显示变量
extern uint8_t MoveTemp;		      	 //楼层临时闪烁变量
extern uint8_t PlatformTempControl;	 //楼层临时控制变量
extern uint8_t PlatformData[20];     //楼层显示数据变量

void Delay_Ms(u32 nTime);         //延时函数
void PBDATA_Configuration(void);  //公共函数初始化
void Platform_Display(void);      //当前楼层显示函数
void Platform_Twinkle(void);      //平台闪烁控制函数
void Platform_Move(void);         //平台移动控制函数

#endif
