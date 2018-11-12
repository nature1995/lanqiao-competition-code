 /*第七届决赛试题*/
 /*完美实现全部内容，无bug*/
 /*作者：龚子然*/

#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "LED.h"
#include "i2c.h"
#include "gong.h"


uint8_t BHH = 0, BMM = 0, BSS = 0;

u8 string[20];
u8 RXbuf[20];
//记录 
u8 T[100];//温度
u8 S[100];//温度
u8 Time1[100];//时间
u8 Time2[100];
u8 Time3[100];
u8 PA1=0,PA2=0;
u8 Receive;
u8 shezhi=0;
u8 qiehuan=0;
u8 RXCUNT=0;
u8 jiange=0;//间隔
u8 nums=0;
u8 show1=0,show2=0;
u8 caiji=0;
u8 i=0;

u16 run = 0xffff;	//LED初始状态，全灭
uint16_t period=666;

uint32_t THH = 0, TMM = 0, TSS = 0;
u32 TimingDelay = 0;
__IO uint32_t TimeDisplay=0;

extern uint32_t TIM3Freq;
float TMax=40,ShiMax=80,interval=1,signal=1500;
float tem,shidu;
float ADC_VALUE;

void TIM2_Configuration(void);
void TIM3_Configuration(void);

void Delay_Ms(u32 nTime);
void KEY_Scan(void);
void Time_Display(uint32_t TimeVar);;
void AD_Read(void);

void IIC_Write(unsigned char add,unsigned char dat);
unsigned char IIC_Read(unsigned char add);
void LCD_Init(void);
void Time_Display(uint32_t TimeVar);
void USART_Send(u8 *str);
void LED_RUN(u16 run);
void List(void);//记录60组数据

int main(void)
{
    SysTick_Config(SystemCoreClock/1000);

		LCD_Init();
		GPIO_Int();
	
    USART2_Configuration();	
		TIM2_Configuration();
		TIM3_Configuration();;
		i2c_init();	

  while (1)
  {
	  	KEY_Scan();
		
	    if(shezhi==0)
				{
				AD_Read();
				List();
				Time_Display(RTC_GetCounter());
				shidu = (TIM3Freq/1000)*80/9.0+10-80/9.0;
				sprintf((char*)string,"%s%.0f%%       "," C Shidu:",shidu);//湿度为float
				LCD_DisplayStringLine(Line4,string);
				sprintf((char*)string," PWM-PA1: %0.2d ", TIM3Freq);
			 	LCD_DisplayStringLine(Line7,string);
	    }
			if(Receive==1)
			{
			   Receive=0;
			   sprintf((char*)string,"Current-TempMax:%.0f Current-Time:%0.2d:%0.2d:%0.2d\r\n", TMax,THH,TMM,TSS); 
			   USART_Send(string); 
		
			}
			if(Receive==2)
			{
			   Receive=0;
			   for(i=0;i<RXCUNT;i++)
			   {   
				   if(T[i]!=0000)
				   {
				   sprintf((char*)string,"Current-Temp:%.0dC Current-Shidu:%.0d%% Time:%0.2d-%0.2d-%0.2d\r\n",T[i],S[i],Time1[i],Time2[i],Time3[i]);
				   USART_Send(string); 
				   }
				   else	if(T[i]==0000)
				   {
//				   sprintf((char*)string,"输出完毕\r\n");
//				   USART_Send(string); 
				   }			  
			   }
			 
			}
		
			if(tem >=TMax )
			{
			 	    run = run & 0xfeff;	 //第1个灯打开
			     	LED_RUN(run);
			} 
			else
			{
				   run = run |0x0100;	 //第1个灯关闭
				   LED_RUN(run);					 
			}
			if(tem >ShiMax )
			{
			 	    run = run & 0xfdff;	 //第2个灯打开
			     	LED_RUN(run);
			} 
			else
			{
				   run = run |0x0200;
				   LED_RUN(run);		//第2个灯关闭
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

void AD_Read(void){

	   ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
	   ADC_VALUE = ADC_GetConversionValue(ADC1)*3.3/0xFFF;
	   tem=(ADC_VALUE*80)/3.3-20;
	   if(tem>=60){
	    tem=60;	   
	   }

	   run = run & 0xfbff;
	   LED_RUN(run);
	   Delay_Ms(50);
	   run = run |0x0400;
     LED_RUN(run);

	   sprintf((char*)string,"%s%.0fC      "," C Temp:",tem);//温度为float
	   LCD_DisplayStringLine(Line3,string);

	  	    
}

void LED_RUN(u16 run)
{
     
	  GPIO_SetBits(GPIOC,run);
	  GPIO_SetBits(GPIOD,GPIO_Pin_2);
	  GPIO_ResetBits(GPIOD,GPIO_Pin_2);

}

void List(void)
{
   	 if(jiange==interval*8)
	 {	 
	   jiange=0;
	   caiji=1;
	   if(caiji==1)
	   {
	   caiji=0;
			if (RXCUNT<=60)
			{
				 	 T[RXCUNT]=tem;//记录
			     S[RXCUNT]=shidu;
			     Time1[RXCUNT]=THH;	 
				 	 Time2[RXCUNT]=TMM;	
				 	 Time3[RXCUNT]=TSS;	
				    
			     RXCUNT++;
				 	 sprintf((char*)string,"           Nums:%d  ",RXCUNT);
				   LCD_DisplayStringLine(Line8,string);
			}
			else 
		  {
			    LCD_DisplayStringLine(Line8,(unsigned char*)"           Nums:Full");
			}
		}
	}
	else
	{
	  jiange++;
	}
      


}


void KEY_Scan(void)
{
    if(RB1==0)
   {
   	  Delay_Ms(10);
		 		if(RB1==0)
   		  {
		   
				  if(shezhi == 0)
					{
						LCD_Clear(Blue);		 
						shezhi= 1;
					  LCD_DisplayStringLine(Line1,(unsigned char*)"      Setting       ");
						sprintf((char*)string," T    Max:%.0fC   ",TMax);
						LCD_DisplayStringLine(Line3,string);
						sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
						LCD_DisplayStringLine(Line4,string);
						sprintf((char*)string," Interval:%.0fS   ",interval);
						LCD_DisplayStringLine(Line5,string);
						sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
						LCD_DisplayStringLine(Line6,string);
						shezhi = 1;					
					}

					else if(shezhi == 1)
					{
				     	LCD_Clear(Blue);
					    shezhi = 0;
				     	LCD_DisplayStringLine(Line1,(unsigned char*)"    Real Data       ");
					    sprintf((char*)string,"%s%.0f"," C Temp:",tem);
			  			LCD_DisplayStringLine(Line3,string);
							LCD_DisplayStringLine(Line4,(unsigned char*)" C Shidu:           ");
							LCD_DisplayStringLine(Line3,string);
					    sprintf((char*)string," T: %0.2d:%0.2d:%0.2d", THH, TMM, TSS);
		          LCD_DisplayStringLine(Line5,string);
					    LCD_DisplayStringLine(Line8,(unsigned char*)"           Nums:    ");
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
		      	if(qiehuan==0|qiehuan==4)
						{
						  LCD_SetBackColor(Blue);
						 	LCD_SetTextColor(Green);
							sprintf((char*)string," T    Max:%.0fC   ",TMax);
							LCD_DisplayStringLine(Line3,string);
							
							LCD_SetTextColor(White);
							sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
							LCD_DisplayStringLine(Line4,string);
							sprintf((char*)string," Interval:%.0fS   ",interval);
							LCD_DisplayStringLine(Line5,string);
							sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
							LCD_DisplayStringLine(Line6,string);
						  qiehuan=1;
				    }
			
						else if (qiehuan==1)
						{	
						  LCD_SetBackColor(Blue);
							LCD_SetTextColor(Green);
							sprintf((char*)string," Shi  Max:%.0f",ShiMax);
							LCD_DisplayStringLine(Line4,string);
			
							LCD_SetTextColor(White);
							sprintf((char*)string," T    Max:%.0fC   ",TMax);
							LCD_DisplayStringLine(Line3,string);
							sprintf((char*)string," Interval:%.0fS   ",interval);
							LCD_DisplayStringLine(Line5,string);
							sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
							LCD_DisplayStringLine(Line6,string);
				
					    qiehuan=2;
						}
						else if (qiehuan==2)
						{
						  LCD_SetBackColor(Blue);
						  LCD_SetTextColor(Green);
							sprintf((char*)string," Interval:%.0fS   ",interval);
							LCD_DisplayStringLine(Line5,string);
			
						  LCD_SetTextColor(White);
							sprintf((char*)string," T    Max:%.0fC   ",TMax);
							LCD_DisplayStringLine(Line3,string);
							sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
							LCD_DisplayStringLine(Line4,string);
							sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
							LCD_DisplayStringLine(Line6,string);
						
					    qiehuan=3;
						}
						else if (qiehuan==3)
						{
						  LCD_SetBackColor(Blue);
						  LCD_SetTextColor(Green);
							sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
							LCD_DisplayStringLine(Line6,string);
			
						  LCD_SetTextColor(White);
							sprintf((char*)string," T    Max:%.0fC   ",TMax);
							LCD_DisplayStringLine(Line3,string);    
							sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
							LCD_DisplayStringLine(Line4,string);
							sprintf((char*)string," Interval:%.0fS   ",interval);
							LCD_DisplayStringLine(Line5,string);
						
					    qiehuan=4;
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
			    	if(qiehuan==1)
						{
					   		TMax++;
							 	if(TMax>=60)TMax=60;
							 	LCD_SetBackColor(Blue);
							 	LCD_SetTextColor(Green);
								sprintf((char*)string," T    Max:%.0fC   ",TMax);
								LCD_DisplayStringLine(Line3,string);
								
								LCD_SetTextColor(White);
								sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
								LCD_DisplayStringLine(Line4,string);
								sprintf((char*)string," Interval:%.0fS   ",interval);
								LCD_DisplayStringLine(Line5,string);
								sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
								LCD_DisplayStringLine(Line6,string);

								Delay_Ms(5);
								IIC_Write(0x00,TMax);
								
						
					   }
					   else if(qiehuan==2)
					   {
					     	ShiMax=ShiMax+5;
							  if(ShiMax>90)ShiMax=0;

						   	LCD_SetBackColor(Blue);
								LCD_SetTextColor(Green);
								sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
								LCD_DisplayStringLine(Line4,string);
				
								LCD_SetTextColor(White);
								sprintf((char*)string," T    Max:%.0fC   ",TMax);
								LCD_DisplayStringLine(Line3,string);
								sprintf((char*)string," Interval:%.0fS   ",interval);
								LCD_DisplayStringLine(Line5,string);
								sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
								LCD_DisplayStringLine(Line6,string);

								Delay_Ms(5);
								IIC_Write(0x01,ShiMax);
					   }
					   else if(qiehuan==3)
					   {
					     	interval=interval+1;
								if(signal>=10)signal=10;
					      LCD_SetBackColor(Blue);
						  	LCD_SetTextColor(Green);
								sprintf((char*)string," Interval:%.0fS   ",interval);
								LCD_DisplayStringLine(Line5,string);
			
						    LCD_SetTextColor(White);
								sprintf((char*)string," T    Max:%.0fC   ",TMax);
								LCD_DisplayStringLine(Line3,string);
								sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
								LCD_DisplayStringLine(Line4,string);
								sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
								LCD_DisplayStringLine(Line6,string);

								Delay_Ms(5);
								IIC_Write(0x01,signal);	
					   }
					    else if(qiehuan==4)
					   {
					     	signal=signal+500;
							 	if(signal>=10000)signal=10000;

						   	LCD_SetBackColor(Blue);
							  LCD_SetTextColor(Green);
								sprintf((char*)string,"   Signal:%0.1fKHz       ",signal*0.001);
								LCD_DisplayStringLine(Line6,string);
				
							  LCD_SetTextColor(White);
								sprintf((char*)string," T    Max:%.0fC   ",TMax);
								LCD_DisplayStringLine(Line3,string);
								sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
								LCD_DisplayStringLine(Line4,string);
								sprintf((char*)string," Interval:%.0fS        ",interval);
								LCD_DisplayStringLine(Line5,string);;

								period = 1000000/signal-1;
					      TIM2_Configuration();

								Delay_Ms(5);
								IIC_Write(0x03,signal);
							
					   }
		  }
		while(!RB3);
        }
  }

    if(RB4==0)
   {
   	  Delay_Ms(10);
		  if(RB4==0)
   		  {		
		   		if(shezhi==1)
		   			{		  
			    		if(qiehuan==1)
						  {
				   	 			TMax--;
							 	  if(TMax<-20) TMax=-20;
									
									LCD_SetBackColor(Blue);
								 	LCD_SetTextColor(Green);
									sprintf((char*)string," T    Max:%.0fC   ",TMax);
									LCD_DisplayStringLine(Line3,string);
									
									LCD_SetTextColor(White);
									sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
									LCD_DisplayStringLine(Line4,string);
									sprintf((char*)string," Interval:%.0fS    ",interval);
									LCD_DisplayStringLine(Line5,string);
									sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
									LCD_DisplayStringLine(Line6,string);

									Delay_Ms(5);
									IIC_Write(0x00,TMax);							 		
					
					   }
						   else if(qiehuan==2)
						   {
						     	ShiMax=ShiMax-5;
								  if(ShiMax<=0)ShiMax=90;

							    LCD_SetBackColor(Blue);
									LCD_SetTextColor(Green);
									sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
									LCD_DisplayStringLine(Line4,string);
					
									LCD_SetTextColor(White);
									sprintf((char*)string," T    Max:%.0fC   ",TMax);
									LCD_DisplayStringLine(Line3,string);
									sprintf((char*)string," Interval:%.0fS   ",interval);
									LCD_DisplayStringLine(Line5,string);
									sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
									LCD_DisplayStringLine(Line6,string);

									Delay_Ms(5);
									IIC_Write(0x01,ShiMax);
						   }
						   else if(qiehuan==3)
						   {
						      interval=interval-1;
									if(signal<=1)signal=1;
						    	LCD_SetBackColor(Blue);
							  	LCD_SetTextColor(Green);
									sprintf((char*)string," Interval:%.0fS   ",interval);
									LCD_DisplayStringLine(Line5,string);
				
							    LCD_SetTextColor(White);
									sprintf((char*)string," T    Max:%.0fC   ",TMax);
									LCD_DisplayStringLine(Line3,string);
									sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
									LCD_DisplayStringLine(Line4,string);
									sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
									LCD_DisplayStringLine(Line6,string);

									Delay_Ms(5);
									IIC_Write(0x02,signal);	 	
						   }
						    else if(qiehuan==4)
						   {
						     	signal=signal-500;
								 	if(signal<=1000)signal=1000;
							   	LCD_SetBackColor(Blue);
								  LCD_SetTextColor(Green);
									sprintf((char*)string,"   Signal:%0.1fKHz   ",signal*0.001);
									LCD_DisplayStringLine(Line6,string);
					
								  LCD_SetTextColor(White);
									sprintf((char*)string," T    Max:%.0fC   ",TMax);
									LCD_DisplayStringLine(Line3,string);
									sprintf((char*)string," Shi  Max:%.0f%%   ",ShiMax);
									LCD_DisplayStringLine(Line4,string);
									sprintf((char*)string," Interval:%.0fS   ",interval);
									LCD_DisplayStringLine(Line5,string);

									period = 1000000/signal-1;
						      TIM2_Configuration();

									Delay_Ms(5);
									IIC_Write(0x03,signal);
						   }
					  }
           }
		 while(!RB4);
   }
}


void Time_Display(uint32_t TimeVar)
{
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
		  	 
		  }
	
		}
	  }
		sprintf((char*)string," T: %0.2d-%0.2d-%0.2d", THH, TMM, TSS);
		LCD_DisplayStringLine(Line5,string);
		AD_Read();
}



void LCD_Init(void)
{
    STM3210B_LCD_Init();
		LCD_Clear(Blue);
		LCD_SetBackColor(Blue);
		LCD_SetTextColor(White);

		LCD_DisplayStringLine(Line0,(unsigned char*)"                    ");	
		LCD_DisplayStringLine(Line1,(unsigned char*)"    Real Data       ");
    LCD_DisplayStringLine(Line2,(unsigned char*)"                    ");	
		LCD_DisplayStringLine(Line3,(unsigned char*)" C Temp:	           ");
		LCD_DisplayStringLine(Line4,(unsigned char*)" C Shidu:           ");						
		LCD_DisplayStringLine(Line5,(unsigned char*)" T: 12-50-00        ");	
		LCD_DisplayStringLine(Line6,(unsigned char*)"                    ");		
		LCD_DisplayStringLine(Line7,(unsigned char*)"                    ");		
    LCD_DisplayStringLine(Line8,(unsigned char*)"           Nums:    ");
    LCD_DisplayStringLine(Line9,(unsigned char*)"                    ");
}


void Delay_Ms(u32 nTime)
{
		TimingDelay = nTime;
		while(TimingDelay != 0);	
}


void TIM3_Configuration(void)
{  
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_ICInitTypeDef  TIM_ICInitStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	  TIM_TimeBaseStructure.TIM_Period = 65535;
	  TIM_TimeBaseStructure.TIM_Prescaler = 71;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	  TIM_ICInitStructure.TIM_ICFilter = 0x8;
	  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	  TIM_Cmd(TIM3, ENABLE);
	  TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
}

void TIM2_Configuration(void)
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_OCInitTypeDef  TIM_OCInitStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	  TIM_TimeBaseStructure.TIM_Period = period;
	  TIM_TimeBaseStructure.TIM_Prescaler = 71;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = period/2;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	  TIM_ARRPreloadConfig(TIM2, ENABLE);
	  TIM_Cmd(TIM2, ENABLE);

}

