/*第七届省赛（全部实现）第二版*/
/*作者：龚子然*/

#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "led.h"
#include "i2c.h"
#include "gong.h"


void GPIO_Int(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  ADC_InitTypeDef  ADC_InitStructure;
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  


   //key
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


   //led
    GPIO_InitStructure.GPIO_Pin = LEDALL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
		NVIC_Configuration();
	//adc
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
	
	  /* ADC1 regular channel14 configuration */ 
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_13Cycles5);
	
	  /* Configure high and low analog watchdog thresholds */
	  ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0B00, 0x0300);
	  /* Configure channel14 as the single analog watchdog guarded channel */
	  ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_14);
	  /* Enable analog watchdog on one regular channel */
	  ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
	
	  /* Enable AWD interrupt */
	  ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
	
	  /* Enable ADC1 */
	  ADC_Cmd(ADC1, ENABLE);
	
	  /* Enable ADC1 reset calibration register */   
	  ADC_ResetCalibration(ADC1);
	  /* Check the end of ADC1 reset calibration register */
	  while(ADC_GetResetCalibrationStatus(ADC1));
	
	  /* Start ADC1 calibration */
	  ADC_StartCalibration(ADC1);
	  /* Check the end of ADC1 calibration */
	  while(ADC_GetCalibrationStatus(ADC1));
		//rtc
		PWR_BackupAccessCmd(ENABLE);
	    BKP_DeInit();
	 	RCC_LSICmd(ENABLE);
	  /* Wait till LSE is ready */
	  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	  {}
	  /* Select LSE as RTC Clock Source */
	  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	  /* Enable RTC Clock */
	  RCC_RTCCLKCmd(ENABLE);
	  /* Wait for RTC registers synchronization */
	  RTC_WaitForSynchro();
	  /* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();
	  /* Enable the RTC Second */
	  RTC_ITConfig(RTC_IT_SEC, ENABLE);
	  /* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();
	  /* Set RTC prescaler: set RTC period to 1sec */
	  RTC_SetPrescaler(39999); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	  /* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();
	  /* Change the current time */
	  RTC_SetCounter(HH*3600 + MM*60 + SS);
	  /* Wait until last write operation on RTC registers has finished */
	  RTC_WaitForLastTask();

   //led_off
   GPIO_SetBits(GPIOD, GPIO_Pin_2);
   GPIO_SetBits(GPIOC, LEDALL);
   GPIO_ResetBits(GPIOD, GPIO_Pin_2);

}
void USART2_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
	
	  GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	  
	  GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
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



}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
    /* Enable the USARTz Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure) ;
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
	  val = I2CReceiveByte();
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


void LCD_Init(void)
{
  	STM3210B_LCD_Init();
		LCD_Clear(Blue);
		LCD_SetBackColor(Blue);
		LCD_SetTextColor(White);
	
		LCD_DisplayStringLine(Line0,"                    ");	
		LCD_DisplayStringLine(Line1," V1: 1.84V          ");	
		LCD_DisplayStringLine(Line2,"                    ");
		LCD_DisplayStringLine(Line3," k : 0.1            ");
		LCD_DisplayStringLine(Line4,"                    ");
		LCD_DisplayStringLine(Line5," LED: OFF           ");
		LCD_DisplayStringLine(Line6,"                    ");
		LCD_DisplayStringLine(Line7," T: 23-59-55        ");	
		LCD_DisplayStringLine(Line8,"                    ");
		LCD_DisplayStringLine(Line9,"                   1");
		
}


