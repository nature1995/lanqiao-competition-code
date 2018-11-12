 /*第七届决赛模拟试题*/
 /*作者：龚子然*/

#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "LED.h"
#include "i2c.h"
#include "gong.h"

#define USARTz_RxPin             GPIO_Pin_3
#define ADC1_DR_Address    ((uint32_t)0x4001244C)

u16 ADC_ConvertedValue;	

void GPIO_Int(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		DMA_InitTypeDef  DMA_InitStructure;
		ADC_InitTypeDef  ADC_InitStructure;


		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);


  

    /******************************************KEY INIT***********************************************************/

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	

  	/******************************************KEY INIT***********************************************************/

    GPIO_InitStructure.GPIO_Pin = LEDALL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

	/******************************************ADC_DMA INIT***********************************************************/

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE);
		//GPIO工作模式配置
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//选中管脚PB0
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//设置为模拟输入
		GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化结构体	
		//DMA工作模式配置
		DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;//内存地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = 1;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //内存地址固定
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);	
		DMA_Cmd(DMA1_Channel1, ENABLE);
		//ADC1工作模式配置	
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 //禁止扫描模式，扫描模式用于多通道采集
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
		ADC_InitStructure.ADC_NbrOfChannel = 1;	 	//要转换的通道数目1
		ADC_Init(ADC1, &ADC_InitStructure);
		//ADC时钟配置
		RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_13Cycles5);
		ADC_DMACmd(ADC1, ENABLE);
		ADC_Cmd(ADC1, ENABLE);  
		//ADC校准
		ADC_ResetCalibration(ADC1);
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1)); 

	/******************************************USART INIT***********************************************************/
   

	/******************************************RTC INIT***********************************************************/


	  PWR_BackupAccessCmd(ENABLE);
	  BKP_DeInit();
	  RCC_LSICmd(ENABLE);
	  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	  {}
	  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	  RCC_RTCCLKCmd(ENABLE);
	  RTC_WaitForSynchro();
	  RTC_WaitForLastTask();
	  RTC_ITConfig(RTC_IT_SEC, ENABLE);
	  RTC_WaitForLastTask();
	  RTC_SetPrescaler(39999); 
	  RTC_WaitForLastTask();

	  RTC_SetCounter(HH*3600+MM*60+SS);
 	  RTC_WaitForLastTask();
 
	/******************************************LED Config***********************************************************/
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
	  GPIO_SetBits(GPIOC,LEDALL);
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);

  
}

void TIM2_Configuration(u16 CCR2_Val, u16 CCR3_Val)
{
     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	   TIM_OCInitTypeDef  TIM_OCInitStructure;

	   GPIO_InitTypeDef GPIO_InitStructure;
	   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	   GPIO_Init(GPIOA, &GPIO_InitStructure);

	   TIM_TimeBaseStructure.TIM_Period = 999;
     TIM_TimeBaseStructure.TIM_Prescaler = 71;
     TIM_TimeBaseStructure.TIM_ClockDivision = 0;
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

     TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  	/* PWM1 Mode configuration: Channel2 */
	   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	   TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	   TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	   TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	   /* PWM1 Mode configuration: Channel3 */
	   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	   TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	
	   TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	
	   TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	   TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	   TIM_Cmd(TIM2, ENABLE);
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
