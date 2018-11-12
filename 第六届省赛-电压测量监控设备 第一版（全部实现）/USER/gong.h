#ifndef  __GONG_H__
#define  __GONG_H__

#define		HH	   23
#define		MM	   59
#define		SS	   55

void GPIO_Int(void);
void LCD_Init(void);
void NVIC_Configuration(void);
void IIC_Write(unsigned char add,unsigned char dat);
unsigned char IIC_Read(unsigned char add);



#endif

