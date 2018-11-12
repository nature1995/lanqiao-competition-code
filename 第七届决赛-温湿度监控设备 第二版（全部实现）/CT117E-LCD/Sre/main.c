#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "gong.h"
#include "lcd.h"
#include "i2c.h"

__IO uint32_t TimeDisplay = 0;
u32 TimingDelay = 0;

float temp=0;
float ADC_Value;
float shidu=0;
u8 receive=0;
u8 shezhi=0;
u8 k=0;
u8 string[20];

uint32_t THH = 0, TMM = 0, TSS = 0;
u16 period=999;

void Delay_Ms(u32 nTime);
void Time_Display(uint32_t TimeVar);
void AD_Read(void);
void KEY_Scan(void);
void TIM2_Init(void);	
void TIM3_Init(void);

extern u32 TIM3Freq;

int main(void)
{
		SysTick_Config(SystemCoreClock/1000);
		LCD_Init();
		GPIO_Int();
		i2c_init();
	//	USART_Configuration();
	  TIM2_Init();
		TIM3_Init();
		Delay_Ms(10);
	  IIC_Write(0x01,20);
		Delay_Ms(10);
		k=IIC_Read(0x01);
	  sprintf((char*)string,"%s%d     "," k:",k);//温度为float
	  LCD_DisplayStringLine(Line6,string);
	
	while(1)
	{
	    KEY_Scan();
			if(shezhi==0)
			{
			 	AD_Read();
			  Time_Display(RTC_GetCounter());
				shidu = (TIM3Freq/1000)*80/9.0+10-80/9.0;
				sprintf((char*)string,"%s%u      "," PWM:",TIM3Freq);//温度为float
				LCD_DisplayStringLine(Line7,string);
				sprintf((char*)string,"%s%.0f%%      "," C shidu:",shidu);//温度为float
				LCD_DisplayStringLine(Line4,string);
			}

			if(receive==1)
			{
		     receive=0;
			 	 USART_Send((unsigned char*)"OK\n");
			
			}
	}
}

void KEY_Scan(void)
{
   if(RB1==0)
   {
   	  Delay_Ms(10);
	  if(RB1==0)
	  {
	   	LCD_DisplayStringLine(Line7,(unsigned char*)"  1111111111   ");		
	  
	  }
	  while(!RB1);
   
   }

    if(RB2==0)
   {
   	  Delay_Ms(10);
	  if(RB2==0)
	  {
	   	LCD_DisplayStringLine(Line7,(unsigned char*)"  222222222      ");		
	  
	  }
	  while(!RB2);
   
   }

    if(RB3==0)
   {
   	  Delay_Ms(10);
	  if(RB3==0)
	  {
	   	LCD_DisplayStringLine(Line7,(unsigned char*)"    333333333     ");		
	  
	  }
	  while(!RB3);
   
   }

    if(RB4==0)
   {
   	  Delay_Ms(10);
	  if(RB4==0)
	  {
	   	LCD_DisplayStringLine(Line7,(unsigned char*)"    444444444444     ");		
	  
	  }
	  while(!RB4);
   
   }

}

void AD_Read(void)
{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		ADC_Value= ADC_GetConversionValue(ADC1)*3.3/0xfff;
		temp = (ADC_Value*80)/3.3-20;
		sprintf((char*)string,"%s%.0fC      "," C Temp:",temp);//温度为float
		LCD_DisplayStringLine(Line3,string);

}

void Time_Display(uint32_t TimeVar)
{
  
  if (RTC_GetCounter() == 0x0001517F)
  {
     RTC_SetCounter(0x0);
     /* Wait until last write operation on RTC registers has finished */
     RTC_WaitForLastTask();
  }
  
	  /* Compute  hours */
	  THH = TimeVar / 3600;
	  /* Compute minutes */
	  TMM = (TimeVar % 3600) / 60;
	  /* Compute seconds */
	  TSS = (TimeVar % 3600) % 60;
	
	  sprintf((char*)string," T: %0.2d-%0.2d-%0.2d    ", THH, TMM, TSS);
	  LCD_DisplayStringLine(Line5,string);
}

void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    TIM_TimeBaseStructure.TIM_Period = period;	 //1k
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = period>>1;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	  TIM_ARRPreloadConfig(TIM2, ENABLE);
	  TIM_Cmd(TIM2, ENABLE);

}

void TIM3_Init(void)
{
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
   
	  TIM_TimeBaseInitStruct.TIM_Prescaler = 71; //1M
	  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 默认值
	  TIM_TimeBaseInitStruct.TIM_Period = 65535;
	  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // 默认值
	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct); // 初始化 TIM3
	
	  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	
	  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	  
	  /* TIM enable counter */
	  TIM_Cmd(TIM3, ENABLE);
	
	  /* Enable the CC2 Interrupt Request */
	  TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);


}

void Delay_Ms(u32 nTime)
{
		TimingDelay = nTime;
		while(TimingDelay != 0);	
}
