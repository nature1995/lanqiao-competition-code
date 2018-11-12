#ifndef __GONG_H__
#define __GONG_H__	

#define USARTz_RxPin             GPIO_Pin_3
#define USARTz_TxPin             GPIO_Pin_2

void LCD_Init(void);
void GPIO_Int(void);
void NVIC_Configuration(void);
unsigned char IIC_Read(unsigned char add);
void IIC_Write(unsigned char add,unsigned char data);
void USART_Configuration(void);

#endif

