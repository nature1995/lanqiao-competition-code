#ifndef __RTC_H
#define __RTC_H

#include "pbdata.h"

#define HH 12  //Hour
#define MM 50  //Minute
#define SS 55  //Second

extern uint8_t TimeData[20];     //TIME显示数据变量

void RTC_Configuration(void);    //RTC配置函数
void TIME_Display(void);         //TIME显示函数
void TIME_Read(u32 TimeRead);    //TIME读取函数

#endif
