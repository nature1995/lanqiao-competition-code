#ifndef  __gong_H__
#define  __GONG_H__

#include "stm32f10x.h"

#define x_pos(x)	319 - (x)*16

void GPIO_Int(void);
void LCD_Init(void);
void NVIC_Configuration(void);
void IIC_Write(unsigned char add,unsigned char dat);
unsigned char IIC_Read(unsigned char add);



#endif

