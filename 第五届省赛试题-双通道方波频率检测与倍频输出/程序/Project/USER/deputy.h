#ifndef __DEPUTY_H
#define __DEPUTY_H

#include "stm32f10x.h"

void GPIO_Int(void);
void LCD_Init(void);

unsigned char IIC_Read(unsigned char add);
void IIC_Write(unsigned char add,unsigned char dat);

void USA2_Configuration(void);
void NVIC_Configuration(void);

void TIM3_Configuration(void);
void TIM2_Configuration(void);


#endif

