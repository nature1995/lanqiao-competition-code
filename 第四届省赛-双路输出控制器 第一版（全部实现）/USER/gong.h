#ifndef  __gong_H__
#define  __GONG_H__

 #include "stm32f10x.h"

#define		HH	   23
#define		MM	   59
#define		SS	   50

void GPIO_Int(void);
void LCD_Init(void);
void NVIC_Configuration(void);
void IIC_Write(unsigned char add,unsigned char dat);
unsigned char IIC_Read(unsigned char add);

void TIM2_Configuration(u16 CCR2_Val, u16 CCR3_Val);
void USART2_Configuration(void);


#endif

