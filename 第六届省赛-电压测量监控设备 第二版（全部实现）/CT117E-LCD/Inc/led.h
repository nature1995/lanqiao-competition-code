#ifndef __LED_H
#define __LED_H

#define  HH	 23
#define  MM	 59
#define  SS	 55


#define LED1 GPIO_Pin_8                
#define LED2 GPIO_Pin_9                 
#define LED3 GPIO_Pin_10                
#define LED4 GPIO_Pin_11                
#define LED5 GPIO_Pin_12                
#define LED6 GPIO_Pin_13                
#define LED7 GPIO_Pin_14               
#define LED8 GPIO_Pin_15                
#define LEDALL GPIO_Pin_All

#define	 RB1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define	 RB2 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)
#define	 RB3 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)
#define	 RB4 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)               
#endif


