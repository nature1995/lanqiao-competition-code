#ifndef __KEY_H
#define __KEY_H

#include "pbdata.h"

#define KEY1_IN	  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY2_IN	  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define KEY3_IN   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define KEY4_IN   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)

extern uint8_t KeyOne;         //KEY1控制变量
extern uint8_t KeyTwo;         //KEY2控制变量
extern uint8_t KeyThree;       //KEY3控制变量
extern uint8_t KeyFour;        //KEY4控制变量
extern uint8_t Key1Control;    //KEY1读取变量
extern uint8_t Key2Control;	   //KEY2读取变量
extern uint8_t Key3Control;    //KEY3读取变量
extern uint8_t Key4Control;    //KEY4读取变量
extern uint8_t KeyControl;     //KEY使能控制变量
extern uint32_t KeyTiming;     //KEY定时控制变量
extern uint8_t KeyScanContuol; //KEY扫描控制变量

uint8_t KeyScan(void);             //KEY扫描函数
void KEY_Configuration(void);      //KEY配置函数
void KEY_Control(uint8_t KeyTemp); //KEY使能函数

#endif
