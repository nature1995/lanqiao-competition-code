#ifndef __GONG_H
#define __GONG_H

#define USARTz_RxPin             GPIO_Pin_3
#define USARTz_TxPin             GPIO_Pin_2

void GPIO_Int(void);
void LCD_Init(void);
void NVIC_Configuration(void);
void USART_Configuration(void);
void USART_Send(u8 *str);
unsigned char IIC_Read(unsigned char add);	
void IIC_Write(unsigned char add,unsigned char data);

#endif

