/*第六届省赛试题（已实现所有）第一版*/
/*作者：龚子然*/
#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "LED.h"
#include "i2c.h"
#include "gong.h"


__IO uint32_t TimeDisplay=0;

u8 string[20];
u8 TXbuf[20];

u8  AD_Flag;
u8 miao;
u8 k;
u8 Receive =0 ;
u8 Receive_z =0 ;

u8 LED_shan = 1;
u8 MS200 = 0;
u8 LED1_kai = 0;
u8 shezhi = 0;
u8 qiehuan = 0;
u8 LED_baojing = 0;

u32 TimingDelay = 0;

uint8_t BHH = 0, BMM = 0, BSS = 0;
uint32_t THH = 0, TMM = 0, TSS = 0;

float ADC_VALUE; 

void Delay_Ms(u32 nTime);
void KEY_Scan(void);
void Time_Display(uint32_t TimeVar);
void AD_Read(void);

void IIC_Write(unsigned char add,unsigned char dat);
unsigned char IIC_Read(unsigned char add);
void LCD_Init(void);
void NVIC_Configuration(void);
void USART_Send(u8 *str);
void SysTick_Handler(void);


int main(void)
{
		  SysTick_Config(SystemCoreClock/1000);
			LCD_Init();
			GPIO_Int();
			i2c_init();
		  if(IIC_Read(0xff)!=20)
			{
					Delay_Ms(5);
					IIC_Write(0x00,1);
					Delay_Ms(5);
					IIC_Write(0xff,20);
			}

			Delay_Ms(5);
			k=IIC_Read(0x00);
			sprintf((char*)string," k : 0.%0.1d", k);
			LCD_DisplayStringLine(Line3,string);

  while (1)
  {
  	KEY_Scan();
	 	if(shezhi==0){
		  	AD_Read();
	 		 if(TimeDisplay==1)
	      {
					Time_Display(RTC_GetCounter());
					TimeDisplay=0;
				
				  if(Receive==1)
					{
				    	 Receive = 0;
				  		 k=Receive_z-48;
				   		 sprintf((char*)string," k : 0.%0.1d", k);
							 LCD_DisplayStringLine(Line3,string);
							 Delay_Ms(5);
							 IIC_Write(0x00,k);
							 USART_Send((unsigned char*)"ok\n");
				   }	
	      }	
	 }
  }
}

void USART_Send(u8 *str)
{
	  int index=0;
	  do{
	   USART_SendData(USART2, str[index++]);
	   while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	   }
	   while(str[index]!=0);
}

void KEY_Scan(void)
{
    if(RB1==0)
   {
  	  Delay_Ms(10);
	      if(RB1==0)
	   		 { 	      
				if(LED_shan==1)
				{
				LED_shan=0;
				LCD_DisplayStringLine(Line5,(unsigned char*)" LED: OFF          ");
				} 
				else if(LED_shan==0)
				{
				LED_shan=1;
				LCD_DisplayStringLine(Line5,(unsigned char*)" LED: ON           ");
				}
			 
		 
	 }
		  while(!RB1);
   }

    if(RB2==0)
   {
   	  Delay_Ms(10);
		 if(RB2==0)
   		  {
		  	if(shezhi == 0)
				{
	        	LCD_Clear(Blue);
				   	LCD_DisplayStringLine(Line3,(unsigned char*)"      Setting       ");
						LCD_DisplayStringLine(Line9,(unsigned char*)"                   2");						
						sprintf((char*)string,"      %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
						LCD_DisplayStringLine(Line5,string);
						shezhi = 1;
				}
			else if(shezhi == 1)
				{
			     	LCD_Clear(Blue);
				    shezhi = 0;
						sprintf((char*)string," k : 0.%0.1d         ", k);
						LCD_DisplayStringLine(Line3,string);
						LCD_DisplayStringLine(Line9,(unsigned char*)"                   1");
	
				    if(LED_shan==1)
						{
						LCD_DisplayStringLine(Line5,(unsigned char*)" LED: ON             ");
						} 
						else if(LED_shan==0)
						{
						LCD_DisplayStringLine(Line5,(unsigned char*)" LED: OFF           ");	  
						}		
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
						 if(qiehuan ==3)
							 {
							  qiehuan = 0;
							 }

						 if(qiehuan == 0)
						   {
						   	    sprintf((char*)string,"      %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
						        LCD_DisplayStringLine(Line5,string); 	
					  	      LCD_DisplayStringLine(Line6,(unsigned char*)"      ~~                ");
										qiehuan = 1;
						   }

						 else if(qiehuan == 1)
						   {
						   	    sprintf((char*)string,"      %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
					        	LCD_DisplayStringLine(Line5,string); 
					  	      LCD_DisplayStringLine(Line6,(unsigned char*)"         ~~             ");
							  		qiehuan = 2;
						   
						   }

						 else if(qiehuan == 2)
						   {
						   	   sprintf((char*)string,"      %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
					         LCD_DisplayStringLine(Line5,string); 
					  	     LCD_DisplayStringLine(Line6,(unsigned char*)"            ~~          ");
							  	 qiehuan = 3;
						   
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
				 	if(qiehuan == 1)
					{
					  BHH=BHH+1;
					  if(BHH==24)
					  {
					  	BHH =0;
					  }
							sprintf((char*)string,"      %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
							LCD_DisplayStringLine(Line5,string);
				    }
					if(qiehuan == 2)
					{
					   BMM=BMM+1;
					   if(BMM==60)
					  {
					   	BMM = 0;
					  }
						 	sprintf((char*)string,"      %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
							LCD_DisplayStringLine(Line5,string); 
				     }
					if(qiehuan == 3)
					{
					   BSS=BSS+1;
					   if(BSS==60)
					  {
					   BSS = 0;
					  }
						 	sprintf((char*)string,"      %0.2d-%0.2d-%0.2d", BHH, BMM, BSS);
							LCD_DisplayStringLine(Line5,string); 
					}
		
		   }			
		 while(!RB4);	 
      }		
}


void Time_Display(uint32_t TimeVar)
{
   uint32_t THH = 0, TMM = 0, TSS = 0;
  
	  THH = TimeVar / 3600;
	  TMM = (TimeVar % 3600) / 60;
	  TSS = (TimeVar % 3600) % 60;

	  if(THH==BHH) 
	  {
	    if(TMM==BMM)
			{
			  if(TSS==BSS)
			  {
			  	sprintf((char*)TXbuf,"%s%.2f+0.%0.1d+ %0.2d%0.2d%0.2d\n","",ADC_VALUE,k,THH,TMM,TSS);
					USART_Send(TXbuf);
			  }
			}
	  }

  	sprintf((char*)string," T: %0.2d-%0.2d-%0.2d", THH, TMM, TSS);
		LCD_DisplayStringLine(Line7,string);
}


void AD_Read(void){

	   ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
	   ADC_VALUE = ADC_GetConversionValue(ADC1)*3.3/0xFFF;
	   sprintf((char*)string,"%s%.2fV"," V1: ",ADC_VALUE);
	   LCD_DisplayStringLine(Line1,string);

	   if(ADC_VALUE>k*3.3*0.1)
	   {
	   	LED_baojing=1;
	   }
	   else
	   {
	   	LED_baojing=0;
	   }

}

void SysTick_Handler(void)
{
	TimingDelay--;
		if(LED_baojing == 1)
		{
			if(LED_shan == 1)
			{
			  if(++MS200 == 200)
				{
			    MS200=0;
			    if(LED1_kai == 0 )
					{	
				    LED1_kai = 1;
		         	GPIO_SetBits(GPIOD,GPIO_Pin_2);
		         	GPIO_SetBits(GPIOC,LED2|LED3|LED4|LED5|LED6|LED7|LED7|LED8);
			        GPIO_ResetBits(GPIOC,LED1);
			        GPIO_ResetBits(GPIOD,GPIO_Pin_2);
		            
					}
		     	else
		     	{   
			        LED1_kai = 0;
			      	GPIO_SetBits(GPIOD,GPIO_Pin_2);
			    		GPIO_SetBits(GPIOC,LEDALL);
			 	    	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				
			    }		 	
			}
			
			}
			else if (LED_shan == 0)
			{
			        LED1_kai = 0;
			      	GPIO_SetBits(GPIOD,GPIO_Pin_2);
			    		GPIO_SetBits(GPIOC,LEDALL);
			 	   	  GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			
			}
		}
}

void LCD_Init(void)
{
    STM3210B_LCD_Init();
		LCD_Clear(Blue);
		LCD_SetBackColor(Blue);
		LCD_SetTextColor(White);
	
		LCD_DisplayStringLine(Line0,(unsigned char*)"                    ");	
		LCD_DisplayStringLine(Line1,(unsigned char*)" V1: 1.84V          ");	
		LCD_DisplayStringLine(Line2,(unsigned char*)"                    ");
		LCD_DisplayStringLine(Line3,(unsigned char*)" k : 0.1            ");
		LCD_DisplayStringLine(Line4,(unsigned char*)"                    ");						
		LCD_DisplayStringLine(Line5,(unsigned char*)" LED: ON            ");	
		LCD_DisplayStringLine(Line6,(unsigned char*)"                    ");	
		LCD_DisplayStringLine(Line7,(unsigned char*)" T: 23-59-55        ");		
		LCD_DisplayStringLine(Line8,(unsigned char*)"                    ");		
		LCD_DisplayStringLine(Line9,(unsigned char*)"                   1");

}

void Delay_Ms(u32 nTime)
{
		TimingDelay = nTime;
		while(TimingDelay != 0);	
}

