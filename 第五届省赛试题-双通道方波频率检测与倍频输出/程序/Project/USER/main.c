/*******************************************************************************  
* 文件名称：第五届省赛试题--双通道方波频率检测与倍频输出
* 掌握内容：考场如何复制ST官网驱动；外部EEPROM；串口通信，PWM波捕获和比较输出
* 程序说明：如需了解有关本程序配套视频讲解，及比赛经验分享\
*					 欢迎访问淘宝店铺网址：shop117015787.taobao.com
* 作者：shop117015787.taobao.com
* 日期版本：2016-12
*******************************************************************************/


#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "lcd.h"
#include "i2c.h"
#include "DEPUTY.h"

u32 TimingDelay = 0;
u8 string[20];
u8 PA6=2,PA7=4;
u8 RXbuf[20];
u8 Receive;

uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0,IC3ReadValue3 = 0, IC3ReadValue4 = 0;
uint32_t Capture1 = 0,Capture2 = 0;
uint32_t TIM3Freq2 = 0,TIM3Freq2_last = 0,TIM3Freq3 = 0,TIM3Freq3_last = 0;
uint16_t show1=0,show2=0;

u8 bendi=0,qiehuan=0;
u16 run=0xfbff;

u32 CCR1_Val = 500;//1KHZ
u32 CCR2_Val = 10;//50KHZ
u8 PA6_OUT=0,PA7_OUT=0;

void Delay_Ms(u32 nTime);
void KEY_Scan(void);
void LED_RUN(u16 run);

int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	
	LCD_Init();
	GPIO_Int();
	
	i2c_init();
//	Delay_Ms(5);
//	IIC_Write(0x00,2);
//	Delay_Ms(5);
//	IIC_Write(0x01,2);
	Delay_Ms(5);
	PA6=IIC_Read(0x00);
	Delay_Ms(5);
	PA7=IIC_Read(0x01);
	
	sprintf((char*)string," N(1): %0.1d", PA6);
	LCD_DisplayStringLine(Line2,string);	
	
	sprintf((char*)string," N(2): %0.1d", PA7);
	LCD_DisplayStringLine(Line5,string);	
	
	USA2_Configuration();
	TIM2_Configuration();
	TIM3_Configuration();
	
  while (1)
  {
		KEY_Scan();
		LED_RUN(run);
		
		if(Receive==1)
		{
			Receive=0;
			LCD_DisplayStringLine(Line8,RXbuf);
			if((RXbuf[4]-48)==1)
			{
				PA6=RXbuf[6]-48+RXbuf[7]-48;
				sprintf((char*)string," N(1): %0.1d", PA6);
				LCD_DisplayStringLine(Line2,string);
				Delay_Ms(5);
				IIC_Write(0x00,PA6);
			}
			else if((RXbuf[4]-48)==2)
			{
				PA7=RXbuf[6]-48+RXbuf[7]-48;
				sprintf((char*)string," N(2): %0.1d", PA7);
				LCD_DisplayStringLine(Line5,string);	
				Delay_Ms(5);
				IIC_Write(0x01,PA7);
			}
		}
		
		if(show1==1)
		{
			show1=0;
			sprintf((char*)string," Channel(1):%0.2dHz   ", TIM3Freq2);
			LCD_DisplayStringLine(Line1,string);
			
			if((TIM3Freq2>=50)&&(TIM3Freq2<=50000))
			{	
				CCR1_Val=500000/TIM3Freq2/PA6;//500000/TIM3Freq2/PA6;
				run=run&0xfeff;
				PA6_OUT=1;
			}
			else 
			{
				run=run|0x0100;	
				PA6_OUT=0;
			}
		}
		else 
		{
			run=run|0x0100;	PA6_OUT=0;
		}	
		if(show2==1)
		{
			show2=0;
			sprintf((char*)string," Channel(2):%0.2dHz    ", TIM3Freq3);
			LCD_DisplayStringLine(Line4,string);

			if((TIM3Freq3>=50)&&(TIM3Freq3<=50000))
			{
				CCR2_Val=500000/TIM3Freq3/PA7;
				run=run&0xfdff;PA7_OUT=1;
			}
			else 
			{
				run=run|0x0200;PA7_OUT=0;
			}			
		}	
		else 
		{
			run=run|0x0200;PA7_OUT=0;
		}
  }
}


void TIM3_IRQHandler(void)
{
	u16 capture;	
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		capture = TIM_GetCapture1(TIM3);
		TIM_SetCompare1(TIM3, capture + CCR1_Val);
		if(PA6_OUT==1)
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)(1-GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6)));
		}
		else 
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)0);
		}
	}
	else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM3);
		TIM_SetCompare2(TIM3, capture + CCR2_Val);
		if(PA7_OUT==1)
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)(1-GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7)));
		}	
		else 
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)0);
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
			if(bendi==0)
			{
				bendi=1;qiehuan=1;
				LCD_DisplayStringLine(Line8,(unsigned char *)"                  1 ");
				USART_Cmd(USART2, DISABLE);
				USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);		
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
				run=run|0x0400;
			}
			else if(bendi==1)
			{
				bendi=0;qiehuan=0;
				USA2_Configuration();
				run=run&0xfbff;
			}
		}
		while(!RB1);
	}
	else 	if(RB2==0)
	{
		Delay_Ms(10);
		if(RB2==0)
		{
			if(bendi==1)
			{
				if(qiehuan==1)
				{
					qiehuan=2;
					LCD_DisplayStringLine(Line8,(unsigned char *)"                  2 ");
				}
				else if(qiehuan==2)
				{
					qiehuan=1;
					LCD_DisplayStringLine(Line8,(unsigned char *)"                  1 ");
				}
			}
		}
		while(!RB2);
	}
	else 	if(RB3==0)
	{
		Delay_Ms(10);
		if(RB3==0)
		{
			if(qiehuan==1)
			{
				PA6--;
				if(PA6<=1)PA6=1;
				sprintf((char*)string," N(1): %0.1d", PA6);
				LCD_DisplayStringLine(Line2,string);	
				Delay_Ms(5);
				IIC_Write(0x00,PA6);

			}
			else if(qiehuan==2)
			{
				PA7--;
				if(PA7<=1)PA7=1;
				sprintf((char*)string," N(2): %0.1d", PA7);
				LCD_DisplayStringLine(Line5,string);
				Delay_Ms(5);
				IIC_Write(0x01,PA7);				
			}
		}
		while(!RB3);
	}
	else 	if(RB4==0)
	{
		Delay_Ms(10);
		if(RB4==0)
		{
			if(qiehuan==1)
			{
				PA6++;
				if(PA6>=10)PA6=10;
				sprintf((char*)string," N(1): %0.1d", PA6);
				LCD_DisplayStringLine(Line2,string);	
				Delay_Ms(5);
				IIC_Write(0x00,PA6);
			}
			else if(qiehuan==2)
			{
				PA7++;
				if(PA7>=10)PA7=10;
				sprintf((char*)string," N(2): %0.1d", PA7);
				LCD_DisplayStringLine(Line5,string);
				Delay_Ms(5);
				IIC_Write(0x01,PA7);					
			}
		}
		while(!RB4);
	}
}

void TIM2_IRQHandler(void)
{ 	
  if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET) 
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
    IC3ReadValue1 = TIM_GetCapture2(TIM2);
  
		if (IC3ReadValue1 != IC3ReadValue2)
		{
			 Capture1 = (IC3ReadValue1 - IC3ReadValue2); 
			 TIM3Freq2 = 1000000/ Capture1;
		}
    if(TIM3Freq2==0)
    {
		  TIM3Freq2=TIM3Freq2_last;
		}
    IC3ReadValue2=IC3ReadValue1 ;
    show1=1;
		TIM3Freq2_last=TIM3Freq2;
  }
	
	
	if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET) 
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    IC3ReadValue3 = TIM_GetCapture3(TIM2);
		
		if (IC3ReadValue3 != IC3ReadValue4)
		{
			 Capture2 = (IC3ReadValue3 - IC3ReadValue4); 
			 TIM3Freq3 = 1000000/ Capture2;
		}   
    IC3ReadValue4=IC3ReadValue3 ;
    show2=1;
  }
}


void LED_RUN(u16 run)
{
	GPIO_SetBits(GPIOC, run);
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}

void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
