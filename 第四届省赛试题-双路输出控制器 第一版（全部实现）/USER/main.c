 /*第四届预赛试题*/
 /*完美实现全部内容，无bug*/
 /*作者：龚子然*/

#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "LED.h"
#include "i2c.h"
#include "gong.h"


u32 TimingDelay = 0;
uint8_t BHH = 0, BMM = 0, BSS = 0;
__IO uint32_t TimeDisplay=0;
u8 string[20];
u8 RXbuf[20];
u8 PA1=0,PA2=0;
u8 Receive;
u16 CCR2_Val;
u16 CCR3_Val;


void Delay_Ms(u32 nTime);
void KEY_Scan(void);
void Time_Display(uint32_t TimeVar);;

void IIC_Write(unsigned char add,unsigned char dat);
unsigned char IIC_Read(unsigned char add);
void LCD_Init(void);
void Time_Display(uint32_t TimeVar);
//void USART_Send(u8 *str);



int main(void)
{
    SysTick_Config(SystemCoreClock/1000);

		LCD_Init();
		GPIO_Int();
//    TIM2_Configuration(500,800);

    USART2_Configuration();
		i2c_init();

		if(IIC_Read(0xff)!=40)
		{
				Delay_Ms(5);
				IIC_Write(0x00,70);
				Delay_Ms(5);
				IIC_Write(0x01,10);
				Delay_Ms(5);

				IIC_Write(0xff,40);
				Delay_Ms(5);
		}
		Delay_Ms(5);
		CCR2_Val =IIC_Read(0x00);
		Delay_Ms(5);
	  CCR3_Val =IIC_Read(0x01);
		Delay_Ms(5);
		sprintf((char*)string," PWM-PA1: %0.2d %%", CCR2_Val);
		LCD_DisplayStringLine(Line1,string);
	  sprintf((char*)string," PWM-PA2: %0.2d %%", CCR3_Val);
		LCD_DisplayStringLine(Line2,string);

  while (1)
  {
  	KEY_Scan();
		if(Receive==1)
		{
		   	Receive=0;
	      LCD_DisplayStringLine(Line6,RXbuf);
	
		}
	  if(TimeDisplay==1)
		{
	 	  TimeDisplay=0;
		  BHH=(RXbuf[3]-48)*10+(RXbuf[4]-48);
		  BMM=(RXbuf[6]-48)*10+(RXbuf[7]-48);
		  BSS=(RXbuf[9]-48)*10+(RXbuf[10]-48);
	
	    Time_Display(RTC_GetCounter());
	
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
		  	if(PA1==0)
					{
					    TIM2_Configuration(CCR2_Val*10,0);
					  	LCD_DisplayStringLine(Line4,(unsigned char*)" Channel: PA1     ");
					    PA1=1;
				    	GPIO_SetBits(GPIOD,GPIO_Pin_2);
			        GPIO_SetBits(GPIOC,LED1|LED3|LED4|LED5|LED6|LED7|LED8);
							GPIO_ResetBits(GPIOC,LED2);
			      	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
					 
					}
				else if(PA1==1)	 
					{
					    TIM2_Configuration(0,0);
				      LCD_DisplayStringLine(Line4,(unsigned char*)" Channel: None      ");
					    PA1=0;
				    	GPIO_SetBits(GPIOD,GPIO_Pin_2);
					    GPIO_SetBits(GPIOC,LEDALL);
							GPIO_ResetBits(GPIOD,GPIO_Pin_2);		
					} 
		 	 }
		  while(!RB1);
   }

    if(RB2==0)
   {
   	  Delay_Ms(10);
		 	if(RB2==0)
   		  {
           CCR2_Val=CCR2_Val+10;
				   if(CCR2_Val>=100)
				   {
				   CCR2_Val=10;	   
				   }   
				   Delay_Ms(5);
				   IIC_Write(0x00,CCR2_Val);
				   sprintf((char*)string," PWM-PA1: %0.2d %%", CCR2_Val);
		       LCD_DisplayStringLine(Line1,string);
					 if(PA1==1)
						{
						   TIM2_Configuration(CCR2_Val*10,0);	 
						}
		  	}
		   while(!RB2);
   }
  
    if(RB3==0)
   {
   	  Delay_Ms(10);
		 if(RB3==0)
   		  {
	  	if(PA2==0)
			{
			    USART_Cmd(USART2, DISABLE);
			    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,DISABLE);
				 
			    TIM2_Configuration(0,CCR3_Val*10);
			  	LCD_DisplayStringLine(Line4,(unsigned char*)" Channel: PA2     ");
			    PA2=1;
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
		      GPIO_SetBits(GPIOC,LED1|LED3|LED4|LED5|LED6|LED7|LED8);
					GPIO_ResetBits(GPIOC,LED2);
		      GPIO_ResetBits(GPIOD,GPIO_Pin_2);

			 
			}
			else if(PA2==1)	 
			{
			    TIM2_Configuration(0,0);
		      LCD_DisplayStringLine(Line4,(unsigned char*)" Channel: None      ");
			    PA2=0;
					USART2_Configuration();
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
			    GPIO_SetBits(GPIOC,LEDALL);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			} 		  
		  }
		    while(!RB3);
   }

    if(RB4==0)
   {
   	  Delay_Ms(10);
		 if(RB4==0)
   		  {		
		 		 	 CCR3_Val=CCR3_Val+10;
				   if(CCR3_Val>=100)
					   	{
					  		 CCR3_Val=10;	   
					    }   
				   Delay_Ms(5);
				   IIC_Write(0x01,CCR3_Val);
				   sprintf((char*)string," PWM-PA2: %0.2d %%", CCR3_Val);
           LCD_DisplayStringLine(Line2,string);
				   if(PA2==1)
							{
							    TIM2_Configuration(0,CCR3_Val*10);	 
							}
		  }
		 while(!RB4);
   }
}

void Time_Display(uint32_t TimeVar)
{
   uint32_t THH = 0, TMM = 0, TSS = 0;
  
  if (RTC_GetCounter() == 0x0001517F)
  {
     RTC_SetCounter(0x0);
     RTC_WaitForLastTask();
  }
  
	  THH = TimeVar / 3600;
	  TMM = (TimeVar % 3600) / 60;
	  TSS = (TimeVar % 3600) % 60;

	  if(THH==BHH) 
	  {
	    if(TMM==BMM)
			{
			  if(TSS==BSS)
			  {
			  	if((RXbuf[14]-48)==1)
						{
								TIM2_Configuration(CCR2_Val*10,0);
					      LCD_DisplayStringLine(Line4,(unsigned char*)" Channel: PA1     ");
								GPIO_SetBits(GPIOD,GPIO_Pin_2);
				        GPIO_SetBits(GPIOC,LED2|LED3|LED4|LED5|LED6|LED7|LED8);
								GPIO_ResetBits(GPIOC,LED1);
				      	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
						}
				else if((RXbuf[14]-48)==2)
						{
								TIM2_Configuration(0,CCR3_Val*10);
							  LCD_DisplayStringLine(Line4,(unsigned char*)" Channel: PA2     ");
								GPIO_SetBits(GPIOD,GPIO_Pin_2);
						    GPIO_SetBits(GPIOC,LED1|LED3|LED4|LED5|LED6|LED7|LED8);
								GPIO_ResetBits(GPIOC,LED2);
						    GPIO_ResetBits(GPIOD,GPIO_Pin_2);
						}
			  }
			  else if (TSS==BSS+RXbuf[16]-48)
					  {
					    TIM2_Configuration(0,0);
							if((RXbuf[14]-48)==2)
								{
								USART2_Configuration();
								}
				     	GPIO_SetBits(GPIOD,GPIO_Pin_2);
					    GPIO_SetBits(GPIOC,LEDALL);
							GPIO_ResetBits(GPIOD,GPIO_Pin_2);
					  }

			}
	  }
		sprintf((char*)string," T: %0.2d:%0.2d:%0.2d", THH, TMM, TSS);
		LCD_DisplayStringLine(Line3,string);
}



void LCD_Init(void)
{
    STM3210B_LCD_Init();
		LCD_Clear(Blue);
		LCD_SetBackColor(Blue);
		LCD_SetTextColor(White);
	
		LCD_DisplayStringLine(Line0,(unsigned char*)"                    ");	
		LCD_DisplayStringLine(Line1,(unsigned char*)" PWM-PA1: 80 %       ");	
		LCD_DisplayStringLine(Line2,(unsigned char*)" PWM-PA2: 10 %       ");
		LCD_DisplayStringLine(Line3,(unsigned char*)" T: 23-59-55        ");						
		LCD_DisplayStringLine(Line4,(unsigned char*)" Channel: None      ");	
		LCD_DisplayStringLine(Line5,(unsigned char*)" Command:           ");		
		LCD_DisplayStringLine(Line6,(unsigned char*)"        None        ");		
		LCD_DisplayStringLine(Line7,(unsigned char*)"                    ");
    LCD_DisplayStringLine(Line8,(unsigned char*)"                    ");
    LCD_DisplayStringLine(Line9,(unsigned char*)"                    ");


}


void Delay_Ms(u32 nTime)
{
		TimingDelay = nTime;
		while(TimingDelay != 0);	
}

