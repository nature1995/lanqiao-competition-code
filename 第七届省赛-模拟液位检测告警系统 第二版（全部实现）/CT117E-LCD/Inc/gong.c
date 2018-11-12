#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "led.h"
#include "gong.h"
#include "i2c.h"

void GPIO_Int(void)
{
	    
		GPIO_InitTypeDef GPIO_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
	
		//KEY                  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	//LED
		GPIO_InitStructure.GPIO_Pin = LEDALL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);


	//ADC
	 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	  ADC_InitStructure.ADC_NbrOfChannel = 1;
	  ADC_Init(ADC1, &ADC_InitStructure);
	
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_13Cycles5);
	
	  ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
	  ADC_Cmd(ADC1, ENABLE);
	
	  ADC_ResetCalibration(ADC1);
	  while(ADC_GetResetCalibrationStatus(ADC1));
	
	  ADC_StartCalibration(ADC1);
	  while(ADC_GetCalibrationStatus(ADC1));

    //LED_OFF
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
	  GPIO_SetBits(GPIOC,LEDALL);
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);



}

unsigned char IIC_Read(unsigned char add)
{
   unsigned char val;
   I2CStart();
   I2CSendByte(0xa0);
	 I2CSendAck();
	 I2CSendByte(add);
	 I2CSendAck();

	 I2CStart();
   I2CSendByte(0xa1);
	 I2CSendAck();
	 val= I2CReceiveByte();
	 I2CSendAck();

	 return(val);

}
void IIC_Write(unsigned char add,unsigned char data)
{
 	 I2CStart();
   I2CSendByte(0xa0);
	 I2CSendAck();
	 I2CSendByte(add);
	 I2CSendAck();
   I2CSendByte(data);
	 I2CSendAck();
	 I2CStop();
}



void USART_Configuration(void)
{

	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef  USART_InitStructure;

    GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
		USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);

    NVIC_Configuration();
   
}


void LCD_Init(void)
{
    STM3210B_LCD_Init();
		LCD_Clear(Blue);
		LCD_SetBackColor(Blue);
		LCD_SetTextColor(White);
	
		LCD_DisplayStringLine(Line0,"                    ");	
		LCD_DisplayStringLine(Line1," Liquid Level       ");	
		LCD_DisplayStringLine(Line2,"                    ");
		LCD_DisplayStringLine(Line3," Height:            ");
		LCD_DisplayStringLine(Line4,"                    ");						
		LCD_DisplayStringLine(Line5," ADC:               ");	
		LCD_DisplayStringLine(Line6,"                    ");	
		LCD_DisplayStringLine(Line7," Level:              ");		
		LCD_DisplayStringLine(Line8,"                    ");		
		LCD_DisplayStringLine(Line9,"                    ");

}

void NVIC_Configuration(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
}


