/*第六届省赛试题by龚子然*/
/*第二版*/


#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "led.h"
#include "i2c.h"
#include "gong.h"


u8 string[20];
u8 Buff[20];
u8 shezhi=0;
u8 receive=0;
u8 k;
u8 Receive_z=0;
u8 LED_1=0;
u8 xuanze=0;


__IO uint32_t TimeDisplay = 0;
uint32_t THH = 0, TMM = 0, TSS = 0;
uint32_t BHH = 0, BMM = 0, BSS = 0;
u32 TimingDelay = 0;


float ADC_VALUE;

void Delay_Ms(u32 nTime);
void KEY_Scan(void);
void Time_Display(uint32_t TimeVar);
void AD_Read(void);
void USART_Send(u8 *str);

int main(void)
{	
	SysTick_Config(SystemCoreClock/1000);
	LCD_Init();
	GPIO_Int();
	USART2_Configuration();
	i2c_init();
	if(IIC_Read(0x30)!=20)
	{
	Delay_Ms(5);
	IIC_Write(0x00,1);
	Delay_Ms(5);
	IIC_Write(0x30,20);
	}
	Delay_Ms(5);
	k=IIC_Read(0x00);
	sprintf((char*)string," k : 0.%d", k);
	LCD_DisplayStringLine(Line3,string);
	
	while(1)
	{
		KEY_Scan();
		if(shezhi==0)
		{
		AD_Read();
		if (TimeDisplay == 1)
	    { 
		   
		  	TimeDisplay = 0;
	      /* Display current time */
	      Time_Display(RTC_GetCounter());
	    
	    }
		}	 
		if(receive==1)
		{ 
		  receive=0;	 
		  k=Receive_z-48;
		  sprintf((char*)string," k : 0.%d", k);
		  LCD_DisplayStringLine(Line3,string);
		  Delay_Ms(5);
		  IIC_Write(0x00,k);
		  USART_Send((unsigned char*)"ok\n");		  	  	
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
		    if(shezhi==0)
			{
		   	  if(LED_1==0)
			  {
			   
			  	  LED_1=1;
				 		LCD_DisplayStringLine(Line5," LED: ON           ");
	      }
			  else if(LED_1==1)
			  {
			   		LED_1=0;
			    	GPIO_SetBits(GPIOD, GPIO_Pin_2);
		        GPIO_SetBits(GPIOC, LEDALL);
		        GPIO_ResetBits(GPIOD, GPIO_Pin_2);
			 			LCD_DisplayStringLine(Line5," LED: OFF           ");
			  }

	   	}
	   }
   while(!RB1);  
   }

    if(RB2==0)
   {
   	Delay_Ms(10);
	   	 if(RB2==0)
	   {
           if(shezhi==0)
			   {
			   	  shezhi=1;
				    LCD_Clear(Blue);
						LCD_SetBackColor(Blue);
						LCD_SetTextColor(White);
					
						LCD_DisplayStringLine(Line0,"                    ");	
						LCD_DisplayStringLine(Line1,"                    ");	
						LCD_DisplayStringLine(Line2,"     Setting        ");
						LCD_DisplayStringLine(Line3,"                    ");
						sprintf((char*)string,"     %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
				   		LCD_DisplayStringLine(Line4,string);
						LCD_DisplayStringLine(Line5,"                    ");
						LCD_DisplayStringLine(Line6,"                    ");
						LCD_DisplayStringLine(Line7,"                    ");	
						LCD_DisplayStringLine(Line8,"                    ");
						LCD_DisplayStringLine(Line9,"                   2");
	
			   }
			   else if(shezhi==1)
			   {
			   	shezhi=0;
					LCD_Clear(Blue);
					LCD_SetBackColor(Blue);
					LCD_SetTextColor(White);
		
					LCD_DisplayStringLine(Line0,"                    ");	
					sprintf((char*)string," V1: %0.2fV    ", ADC_VALUE);
				 	LCD_DisplayStringLine(Line1,string);
					LCD_DisplayStringLine(Line2,"                    ");
					sprintf((char*)string," k : 0.%d", k);
					LCD_DisplayStringLine(Line3,string);
					LCD_DisplayStringLine(Line4,"                    ");
					LCD_DisplayStringLine(Line5," LED: OFF           ");
					LCD_DisplayStringLine(Line6,"                    ");
					sprintf((char*)string," T: %0.2d-%0.2d-%0.2d", THH, TMM, TSS);
				  LCD_DisplayStringLine(Line7,string);		
					LCD_DisplayStringLine(Line8,"                    ");
					LCD_DisplayStringLine(Line9,"                   1");
			   }
		   
	   }
   while(!RB2);  
   }

    if(RB3==0)
   {
   Delay_Ms(10);
	   	 if(RB3==0)
	   {
		     if(shezhi==1)
			 {
				   if(xuanze == 0|xuanze==3)
				   {
				   	  xuanze =1;			 
					 		LCD_DisplayStringLine(Line5,"     ==             ");
				   }
				   else  if(xuanze == 1)
				   {
				   	  xuanze =2;
					 		LCD_DisplayStringLine(Line5,"        ==          ");
				   }
				   else if(xuanze == 2)
				   {
				   	 	xuanze =3;
						 	LCD_DisplayStringLine(Line5,"           ==       ");
				   }
		 
		 }
		 
	   
	   }
   while(!RB3);  
   }

    if(RB4==0)
   {
   	Delay_Ms(10);
	   	 if(RB4==0)
	   {
	   	  if(shezhi==1)
		  {
		  	if(xuanze ==1)
		   	{
				 	if(BHH<24) 
				 	{
					 BHH=BHH+1;			
					 sprintf((char*)string,"     %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
					 	LCD_DisplayStringLine(Line4,string);
				 	}				 	
			 		else if(BHH>=24) BHH=0;
			 	 			 
		   }
		   else  if(xuanze == 2)
		   {
					  if(BMM<60) 
					 {
						 BMM=BMM+1;			
						 sprintf((char*)string,"     %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
						 LCD_DisplayStringLine(Line4,string);
					 }
					 else if(BMM>=60) BMM=0;
				 
			   }
			  else if(xuanze == 3)
			 {
						if(BSS<60) 
						 {
						 	BSS=BSS+1;			
						 	sprintf((char*)string,"     %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
						 	LCD_DisplayStringLine(Line4,string);
						 }
					 else if(BSS>=60) BSS=0;
  			}
		  
		  }
	    
	   
	   }
   while(!RB4); 
   }
}

void USART_Send(u8 *str)
{
	  int index=0; 
	  do{
	  USART_SendData(USART2, str[index++]);
	  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)== RESET); 
	  }
	  while(str[index]!=0);
}

void Time_Display(uint32_t TimeVar)
{ 
  THH = TimeVar / 3600;
  TMM = (TimeVar % 3600) / 60;
  TSS = (TimeVar % 3600) % 60;
  if(THH==BHH)
  {
	  	 if(TMM==BMM)
	  {
		     if(TSS==BSS)
		  {
		  	 sprintf((char*)Buff,"%s%0.2f+0.%d+%0.2d%0.2d%0.2d\r\n","", ADC_VALUE,k,BHH, BMM, BSS);   
			   USART_Send(Buff);
		  
		  }	  
	  } 
  }	  
   sprintf((char*)string," T: %0.2d-%0.2d-%0.2d", THH, TMM, TSS);
   LCD_DisplayStringLine(Line7,string);	

}

void AD_Read(void)
{
	  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	  ADC_VALUE= ADC_GetConversionValue(ADC1)*3.3/0xfff;

    sprintf((char*)string," V1: %0.2fV    ", ADC_VALUE);
   	LCD_DisplayStringLine(Line1,string);
		if(ADC_VALUE>k*3.3*0.1)
		{
			  if(LED_1==1)
			  {
				  
				  	GPIO_SetBits(GPIOD, GPIO_Pin_2);
						GPIO_SetBits(GPIOC, LED2|LED3|LED4|LED5|LED6|LED7|LED8);
		    		GPIO_ResetBits(GPIOC, LED1);
		    		GPIO_ResetBits(GPIOD, GPIO_Pin_2);
						Delay_Ms(200);
						GPIO_SetBits(GPIOD, GPIO_Pin_2);
				 		GPIO_SetBits(GPIOC, LEDALL);
				 		GPIO_ResetBits(GPIOD, GPIO_Pin_2);
						Delay_Ms(200);			 
		  	  }
		}
	if(ADC_VALUE<=3.3*k)
		{	 
			 GPIO_SetBits(GPIOD, GPIO_Pin_2);
			 GPIO_SetBits(GPIOC, LEDALL);
			 GPIO_ResetBits(GPIOD, GPIO_Pin_2);	  	  
		}
}

void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}
