#ifndef  __gong_H__
#define  __GONG_H__

#include "stm32f10x.h"

#define		HH	   12
#define		MM	   50
#define		SS	   00
#define USARTz_RxPin             GPIO_Pin_3
#define USARTz_TxPin             GPIO_Pin_2

void GPIO_Int(void);
void LCD_Init(void);
void NVIC_Configuration(void);
void IIC_Write(unsigned char add,unsigned char dat);
unsigned char IIC_Read(unsigned char add);

void USART2_Configuration(void);


#endif

