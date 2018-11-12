#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "led.h"
#include "gong.h"
#include "i2c.h"

u32 TimingDelay = 0;

u8 string[20];
u8 TXBuf[20];
u8 shezhi=0;
u8 receive=0;
u8 flag_usart=0;
u8 high=0,mid=0,low=0;
u8 level=0;
u8 item=0;

u8 level_last=0;
u8 nums=0;
u16 run=0xffff;

float ADC_VALUE;
int height;
int yunxing=1;

void Delay_Ms(u32 nTime);
void KEY_Scan(void);
void AD_Read(void);
void USART_Send(u8 *str);
void LED2_Run(void);
void LED3_Run(void);

int main(void)
{
	
		SysTick_Config(SystemCoreClock/1000);
		LCD_Init();
		GPIO_Int();
		i2c_init();
		USART_Configuration();

		if(IIC_Read(0xff)!=20){
		   	Delay_Ms(5);
				IIC_Write(0x00,30);
				Delay_Ms(5);
			  IIC_Write(0x01,50);
				Delay_Ms(5);
		    IIC_Write(0x02,70);
	
				Delay_Ms(5);
				IIC_Write(0xff,20);
		}
	
				Delay_Ms(5);
				low=IIC_Read(0x00);
				Delay_Ms(5);
				mid=IIC_Read(0x01);
				Delay_Ms(5);
				high=IIC_Read(0x02);
//	sprintf((char*)string," yuzhi1: %d   ",high);
//   LCD_DisplayStringLine(Line9,string);


	while(1)
	{
	 	 KEY_Scan();
		 if(shezhi==0)
		 {
		   	 AD_Read();		     	 
		 }

	 	 if(yunxing==1)
	   {
		  	GPIO_SetBits(GPIOD,GPIO_Pin_2);
				GPIO_SetBits(GPIOC,LED2|LED3|LED4|LED5|LED6|LED7|LED8);
	    	GPIO_ResetBits(GPIOC,LED1);
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	 	 }
		 else if(yunxing==(-1))
		 {
		  	GPIO_SetBits(GPIOD,GPIO_Pin_2);
				GPIO_SetBits(GPIOC,LED1|LED2|LED3|LED4|LED5|LED6|LED7|LED8);
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);
		 }

		 if(flag_usart==1)
		 { 
		   flag_usart=0;

		   sprintf((char*)TXBuf,"C:H%d+L%d\r\n",height,level);
		   USART_Send(TXBuf);
			 LED3_Run();
		 }
		 else if(flag_usart==2)
		 { 
		   flag_usart=0;
		   
		   sprintf((char*)TXBuf,"S:TL%d+TM%d+TH%d\r\n",low,mid,high);
		   USART_Send(TXBuf);
		   LED3_Run();
		 }	
	}
}

void AD_Read(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_VALUE = ADC_GetConversionValue(ADC1)*3.3/0xfff;
		height = ADC_VALUE*100/3.3;
		sprintf((char*)string," ADC: %0.2fV    ",ADC_VALUE);
		LCD_DisplayStringLine(Line5,string);
		sprintf((char*)string," Height: %dcm   ",height);
		LCD_DisplayStringLine(Line3,string);
		if(height<=low)
		{
			level=0;
		}
		else if(height>=low&&height<=mid)
		{
		    level=1;
		}
		else if(height>=mid&&height<=high)
		{
		    level=2;
		}
		else if(height>=high)
		{
		    level=3;
		}
		sprintf((char*)string," Level: %d   ",level);
	  LCD_DisplayStringLine(Line7,string);
  	if(level!=level_last)
		{
		    if(level<level_last)
				{
				  
				  sprintf((char*)TXBuf,"A:H%d+L%d+D\r\n",height,level);
		      USART_Send(TXBuf);
				  LED2_Run();
				}
				else if(level>level_last)
				{
				  sprintf((char*)TXBuf,"A:H%d+L%d+U\r\n",height,level);
		      USART_Send(TXBuf);
				  LED2_Run();
	
				}
				 	 
		}	
	  level_last=level;
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

void LED2_Run(void)
{
    int i;
		for(i=0;i<5;i++)
		{
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
			GPIO_SetBits(GPIOC,LED1|LED2|LED3|LED4|LED5|LED6|LED7|LED8);
			GPIO_ResetBits(GPIOC,LED2);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			Delay_Ms(200);
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
			GPIO_SetBits(GPIOC,LED1|LED2|LED3|LED4|LED5|LED6|LED7|LED8);
			GPIO_SetBits(GPIOC,LED2);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			Delay_Ms(200);
		}

}

void LED3_Run(void)
{
    int i;
		for(i=0;i<5;i++)
		{
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
	    GPIO_SetBits(GPIOC,LED1|LED2|LED4|LED5|LED6|LED7|LED8);
			GPIO_ResetBits(GPIOC,LED3);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	   	Delay_Ms(200);
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
			GPIO_SetBits(GPIOC,LED1|LED2|LED4|LED5|LED6|LED7|LED8);
	    GPIO_SetBits(GPIOC,LED3);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
		  Delay_Ms(200);
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
					shezhi=1;
					LCD_Clear(Blue);
					LCD_SetBackColor(Blue);
					LCD_SetTextColor(White);
					LCD_DisplayStringLine(Line0,"                  ");	
					LCD_DisplayStringLine(Line1,"  Parameter Setup   ");	
					LCD_DisplayStringLine(Line2,"                    ");
					sprintf((char*)string," Threshold 1: %dcm   ",low);
					LCD_DisplayStringLine(Line3,string);
					LCD_DisplayStringLine(Line4,"                    ");						
					sprintf((char*)string," Threshold 2: %dcm   ",mid);
					LCD_DisplayStringLine(Line5,string);			
					LCD_DisplayStringLine(Line6,"                    ");	
					sprintf((char*)string," Threshold 3: %dcm   ",high);
					LCD_DisplayStringLine(Line7,string);			
					LCD_DisplayStringLine(Line8,"                    ");		
					LCD_DisplayStringLine(Line9,"                    ");
				
				}
			 	else if(shezhi==1)
				{
					shezhi=0;
					LCD_Clear(Blue);
					LCD_SetBackColor(Blue);
					LCD_SetTextColor(White);
					LCD_DisplayStringLine(Line0,"                    ");	
					LCD_DisplayStringLine(Line1," Liquid Level       ");	
					LCD_DisplayStringLine(Line2,"                    ");
					sprintf((char*)string," Height: %dcm   ",height);
					LCD_DisplayStringLine(Line3,string);
					LCD_DisplayStringLine(Line4,"                    ");						
					sprintf((char*)string," ADC: %0.2fV    ",ADC_VALUE);
					LCD_DisplayStringLine(Line5,string);		
					LCD_DisplayStringLine(Line6,"                    ");	
					LCD_DisplayStringLine(Line7," Level:              ");		
					LCD_DisplayStringLine(Line8,"                    ");		
					LCD_DisplayStringLine(Line9,"                    ");
				}	
	}
	while(!RB1);
}

   if(RB2==0)
   {
   	Delay_Ms(10);
    	if(RB2==0)
	    {
	 		 if(shezhi==1)
	  		{
		      if(item==0|item==3)
				  {
						item=1;
						LCD_SetBackColor(Blue);
						LCD_SetTextColor(Green);
						sprintf((char*)string," Threshold 1: %dcm   ",low);
						LCD_DisplayStringLine(Line3,string);
						LCD_SetBackColor(Blue);
						LCD_SetTextColor(White);
						LCD_DisplayStringLine(Line0,"                    ");	
						LCD_DisplayStringLine(Line1,"  Parameter Setup   ");	
						LCD_DisplayStringLine(Line2,"                    ");
						LCD_DisplayStringLine(Line4,"                    ");						
						sprintf((char*)string," Threshold 2: %dcm   ",mid);
						LCD_DisplayStringLine(Line5,string);			
						LCD_DisplayStringLine(Line6,"                    ");	
						sprintf((char*)string," Threshold 3: %dcm   ",high);
						LCD_DisplayStringLine(Line7,string);			
						LCD_DisplayStringLine(Line8,"                    ");		
						LCD_DisplayStringLine(Line9,"                    ");
		
				  }
	   	
	      else if(item==1)
				  {
					 	item=2;
					  LCD_SetBackColor(Blue);
						LCD_SetTextColor(Green);
						sprintf((char*)string," Threshold 2: %dcm   ",mid);
			      LCD_DisplayStringLine(Line5,string);
						LCD_SetBackColor(Blue);
						LCD_SetTextColor(White);
						LCD_DisplayStringLine(Line0,"                    ");	
						LCD_DisplayStringLine(Line1,"  Parameter Setup   ");	
						LCD_DisplayStringLine(Line2,"                    ");
				    sprintf((char*)string," Threshold 1: %dcm   ",low);
			      LCD_DisplayStringLine(Line3,string);
						LCD_DisplayStringLine(Line4,"                    ");														
						LCD_DisplayStringLine(Line6,"                    ");	
					 	sprintf((char*)string," Threshold 3: %dcm   ",high);
			      LCD_DisplayStringLine(Line7,string);			
						LCD_DisplayStringLine(Line8,"                    ");		
						LCD_DisplayStringLine(Line9,"                    ");
				  }	
				  else if(item==2)
				  {
					 	item=3;
						LCD_SetBackColor(Blue);
						LCD_SetTextColor(Green);
						sprintf((char*)string," Threshold 3: %dcm   ",high);
			      LCD_DisplayStringLine(Line7,string);
						LCD_SetBackColor(Blue);
						LCD_SetTextColor(White);
						LCD_DisplayStringLine(Line0,"                    ");	
						LCD_DisplayStringLine(Line1,"  Parameter Setup      ");	
						LCD_DisplayStringLine(Line2,"                    ");
					 	sprintf((char*)string," Threshold 1: %dcm   ",low);
			      LCD_DisplayStringLine(Line3,string);
						LCD_DisplayStringLine(Line4,"                    ");
						sprintf((char*)string," Threshold 2: %dcm   ",mid);
			      LCD_DisplayStringLine(Line5,string);										
						LCD_DisplayStringLine(Line6,"                    ");				
						LCD_DisplayStringLine(Line8,"                    ");		
						LCD_DisplayStringLine(Line9,"                    ");	
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
			   if(item==1)
			   {
					 if(low<mid)
						{
							low+=5;
							LCD_SetBackColor(Blue);
							LCD_SetTextColor(Green);;
							sprintf((char*)string," Threshold 1: %dcm   ",low);
				      LCD_DisplayStringLine(Line3,string);
							Delay_Ms(5);
							IIC_Write(0x00,low);
						}
						else
						{
							low=mid;
							LCD_SetBackColor(Blue);
							LCD_SetTextColor(Green);
							sprintf((char*)string," Threshold 1: %dcm   ",low);
				      LCD_DisplayStringLine(Line3,string);
							Delay_Ms(5);
							IIC_Write(0x00,low);
						}		   
			   }
			   else if(item==2)
			   {
				    if(mid<high)
							{
								mid+=5;
								LCD_SetBackColor(Blue);
								LCD_SetTextColor(Green);
						    sprintf((char*)string," Threshold 2: %dcm   ",mid);
					      LCD_DisplayStringLine(Line5,string);
								Delay_Ms(5);
					      IIC_Write(0x01,mid);
		 					}
						else 
							{
								mid=high;
								LCD_SetBackColor(Blue);
								LCD_SetTextColor(Green);
								sprintf((char*)string," Threshold 2: %dcm   ",mid);
					      LCD_DisplayStringLine(Line5,string);
								Delay_Ms(5);
					      IIC_Write(0x01,mid);
					
				    	}   		
		   		}
			   else if(item==3)
			   {
				  	if(high<100)
				   {
				   	 high+=5;
					 	 LCD_SetBackColor(Blue);
				     LCD_SetTextColor(Green);
				   	 sprintf((char*)string," Threshold 3: %dcm   ",high);
		         LCD_DisplayStringLine(Line7,string);
					   Delay_Ms(5);
		    		 IIC_Write(0x02,high);
					}
					 else
					 {
						 high=100;
						 LCD_SetBackColor(Blue);
						 LCD_SetTextColor(Green);
						 sprintf((char*)string," Threshold 3: %dcm   ",high);
			       LCD_DisplayStringLine(Line7,string);
						 Delay_Ms(5);
			    	 IIC_Write(0x02,high);
					 }	
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
		   if(item==1)
		   {
			    if(low>0)
					{
						low-=5;
						LCD_SetBackColor(Blue);
						LCD_SetTextColor(Green);;
						sprintf((char*)string," Threshold 1: %dcm   ",low);
			            LCD_DisplayStringLine(Line3,string);
						Delay_Ms(5);
						IIC_Write(0x00,low);
					}
			     else
					{
						low=0;
						LCD_SetBackColor(Blue);
						LCD_SetTextColor(Green);
						sprintf((char*)string," Threshold 1: %dcm   ",low);
			            LCD_DisplayStringLine(Line3,string);
						Delay_Ms(5);
						IIC_Write(0x00,low);
					}		   	   
			   }
			   else if(item==2)
			   {
			   		if(mid>low)
						{
						mid-=5;
						LCD_SetBackColor(Blue);
						LCD_SetTextColor(Green);
					  sprintf((char*)string," Threshold 2: %dcm   ",mid);
			      LCD_DisplayStringLine(Line5,string);
						Delay_Ms(5);
			      IIC_Write(0x01,mid);
						}
					else 
					{
						mid=low;
						LCD_SetBackColor(Blue);
						LCD_SetTextColor(Green);
						sprintf((char*)string," Threshold 2: %dcm   ",mid);
			      LCD_DisplayStringLine(Line5,string);
						Delay_Ms(5);
			      IIC_Write(0x01,mid);	
			    	}		
			   }
			   else if(item==3)
			   {
			   	 	if(high>mid)
					   {
					   	 high+=5;
						 	 LCD_SetBackColor(Blue);
					     LCD_SetTextColor(Green);
					   	 sprintf((char*)string," Threshold 3: %dcm   ",high);
			         LCD_DisplayStringLine(Line7,string);
						 	 Delay_Ms(5);
			    		 IIC_Write(0x02,high);
						}
						 else
						 {
							 high=mid;
							 LCD_SetBackColor(Blue);
							 LCD_SetTextColor(Green);
							 sprintf((char*)string," Threshold 3: %dcm   ",high);
				       LCD_DisplayStringLine(Line7,string);
							 Delay_Ms(5);
				       IIC_Write(0x02,high);
						 }		
		   		}
	  	}
		}	
	while(!RB4);
}

}

void Delay_Ms(u32 nTime)
{
		TimingDelay = nTime;
		while(TimingDelay != 0);	
}
