 /*第六届决赛试题——互补输出*/
 /*实现全部内容，无bug*/
 /*作者：龚子然*/

#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "LED.h"
#include "i2c.h"
#include "gong.h"

void GPIO_Int(void)
{
	    GPIO_InitTypeDef GPIO_InitStructure;
			ADC_InitTypeDef  ADC_InitStructure;
		
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

    /******************************************TIM1 INIT***********************************************************/
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       GPIO_Init(GPIOA, &GPIO_InitStructure);

        /*PB13 设置为PWM的反极性输出*/
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       GPIO_Init(GPIOB, &GPIO_InitStructure);

    /******************************************KEY INIT***********************************************************/

	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;
	    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
			NVIC_Configuration();
	
	  	/******************************************LED INIT***********************************************************/
	
	    GPIO_InitStructure.GPIO_Pin = LEDALL;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		/******************************************ADC INIT***********************************************************/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
			ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
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



	 
	/******************************************LED Config***********************************************************/
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
	    GPIO_SetBits(GPIOC,LEDALL);
			GPIO_ResetBits(GPIOC,LED1);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
 
}


void NVIC_Configuration(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure; 
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);   
	  
	  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);


}

unsigned char IIC_Read(unsigned char add){
 
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

void IIC_Write(unsigned char add,unsigned char dat)
{
		 I2CStart();
	   I2CSendByte(0xa0);
		 I2CSendAck();
		 I2CSendByte(add);
		 I2CSendAck();
	   I2CSendByte(dat);
		 I2CSendAck();
		 I2CStop();
}
