 /*第七届决赛模拟试题*/
 /*作者：龚子然*/

#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "LED.h"
#include "i2c.h"
#include "gong.h"


u32 TimingDelay = 0;
u8 string[20];
u8 RXbuf[20];
u8 PA1=0,PA2=0;
u8 Receive;
u8 PA1;
u8 shezhi=0;

u16 CCR2_Val;
u16 CCR3_Val;


float ADC_Value;

void Delay_Ms(u32 nTime);
void KEY_Scan(void);
void AD_Read(void);
void LCD_Init(void);

int main(void)
{
    SysTick_Config(SystemCoreClock/1000);

		LCD_Init();
		GPIO_Int();

		i2c_init();
	  TIM2_Configuration(0,500);

	  Delay_Ms(5);
		IIC_Write(0x00,20);
	  Delay_Ms(5);
		PA1=IIC_Read(0x00);
    sprintf((char*)string,"%s%d"," Votage:",PA1);
	  LCD_DisplayStringLine(Line9,string);


  while (1)
  {
  	KEY_Scan();
		if(shezhi==0)
		{
		 		AD_Read();
		}
  }
}

void AD_Read(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	ADC_Value= ADC_GetConversionValue(ADC1)*3.3/0xfff;
	sprintf((char*)string,"%s%0.2fV   "," ADC: ",ADC_Value);
	LCD_DisplayStringLine(Line1,string);
//		if(B1_flag==0)
//	{
//		sprintf(str,"  Output:OFF");
//		LCD_DisplayStringLine(Line3,(u8 *)str);	
//	}
//	if(B1_flag==1)
//	{
//		sprintf(str,"  Output: ON");
//		LCD_DisplayStringLine(Line3,(u8 *)str);	
//	}
//	sprintf(str,"  Skewing:%3d(deg)",(int)((Read_Adc()/3.3)*360));
//	LCD_DisplayStringLine(Line5,(u8 *)str);	
//	sprintf(str,"  Frequency:%2dKHZ",KHZ);
//	LCD_DisplayStringLine(Line7,(u8 *)str);	
}

void KEY_Scan(void)
{
    if(RB1==0)
   {
   	  Delay_Ms(10);
		 if(RB1==0)
   		  {
		  	
		    }
		  while(!RB1);
   }

    if(RB2==0)
   {
   	  Delay_Ms(10);
		 if(RB2==0)
   		  {
          
		    }
		   while(!RB2);
   }
  
    if(RB3==0)
   {
   	  Delay_Ms(10);
		 if(RB3==0)
   		  {
	  	
		  	}
		    while(!RB3);
   }

    if(RB4==0)
   {
   	  Delay_Ms(10);
		 if(RB4==0)
   		  {		
		  
		  	}
		 while(!RB4);
   }
}




void LCD_Init(void)
{
    STM3210B_LCD_Init();
		LCD_Clear(Blue);
		LCD_SetBackColor(Blue);
		LCD_SetTextColor(White);
	
		LCD_DisplayStringLine(Line0,(unsigned char*)"                    ");	
		LCD_DisplayStringLine(Line1,(unsigned char*)" ADC：              ");	
		LCD_DisplayStringLine(Line2,(unsigned char*)" Output:OFF         ");
		LCD_DisplayStringLine(Line3,(unsigned char*)"                    ");						
		LCD_DisplayStringLine(Line4,(unsigned char*)" P diff:            ");	
		LCD_DisplayStringLine(Line5,(unsigned char*)" Frequency:         ");		
		LCD_DisplayStringLine(Line6,(unsigned char*)"                    ");		
		LCD_DisplayStringLine(Line7,(unsigned char*)"                    ");
    LCD_DisplayStringLine(Line8,(unsigned char*)"                    ");
    LCD_DisplayStringLine(Line9,(unsigned char*)"                    ");


}


void Delay_Ms(u32 nTime)
{
		TimingDelay = nTime;
		while(TimingDelay != 0);	
}

