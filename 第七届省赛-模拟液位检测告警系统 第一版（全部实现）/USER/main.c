#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "LED.h"
#include "i2c.h"

u32 TimingDelay = 0;
u16 miao=0;
u8 string[20];
u8 TX_buf[20];
u8 low,mid,high;
u8 yewei=0,fazhi=0;
u8 shezhi=0,qie=0;
u8 LEVEL;
u8 AD_Flag=0,System_Run=0;
int ADC_VAL; 
BitAction liang = Bit_RESET;


void Delay_Ms(u32 nTime);
void GPIO_Int(void);
void LCD_Init(void);
void KEY_Scan(void);
void AD_Read(void);
void IIC_Write(unsigned char add,unsigned char dat);
unsigned char IIC_Read(unsigned char add);
void NVIC_Configuration(void);
void USART_Send(u8 *str);
void LED2_Run(void);
void LED3_Run(void);

#define USARTz_RxPin             GPIO_Pin_3
#define USARTz_TxPin             GPIO_Pin_2

int main(void)
{
    SysTick_Config(SystemCoreClock/1000);
		LCD_Init();
		GPIO_Int();
		i2c_init();
		Delay_Ms(5);
		if(IIC_Read(0x20)!=20){
		Delay_Ms(5);
		IIC_Write(0x00,30);
		Delay_Ms(5);
	  IIC_Write(0x01,50);
		Delay_Ms(5);
	  IIC_Write(0x02,70);

		Delay_Ms(5);
		IIC_Write(0x20,20);
	
	}
		Delay_Ms(5);
		low=IIC_Read(0x00);
		Delay_Ms(5);
		mid=IIC_Read(0x01);
		Delay_Ms(5);
		high=IIC_Read(0x02);


  while (1)
  {
  	KEY_Scan();
    if(shezhi==0)
		{
			 AD_Read();
	     if(fazhi==1)
	   	{
		    	fazhi=0;
			   	sprintf((char*)TX_buf,"S:TL%2d+TM%2d+TH%2d\r\n",low,mid,high);
			    USART_Send(TX_buf);
					LED3_Run();
	
		  }
			else if(yewei==1)
			{
			   yewei=0;
		     sprintf((char*)TX_buf,"C:H%2d+L%1d\r\n",ADC_VAL,LEVEL);
			   USART_Send(TX_buf);
			   LED3_Run();
			}
	  }
   if(System_Run==1)
	 {

      System_Run=0;

	    if(liang==0)
			{
		    liang=Bit_SET;
				GPIO_SetBits(GPIOD,GPIO_Pin_2);
		    GPIO_SetBits(GPIOC,LED2|LED3|LED4|LED5|LED6|LED7|LED7|LED8);
			  GPIO_ResetBits(GPIOC,LED1);
			  GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	   
	   
	    }
			else
			{
			 liang =Bit_RESET;
			 GPIO_SetBits(GPIOD,GPIO_Pin_2);
			 GPIO_SetBits(GPIOC,LED2|LED3|LED4|LED5|LED6|LED7|LED7|LED8);
		   GPIO_SetBits(GPIOC,LED1);
			 GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	   
	   
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
			    if(shezhi==0)
					{
				  shezhi=1;
			    LCD_DisplayStringLine(Line1,(unsigned char*)"    Parameter Setup  ");
					sprintf((char*)string,"Threshold 1:%2dcm",low);
					LCD_DisplayStringLine(Line3,string);
					sprintf((char*)string,"Threshold 2:%2dcm",mid); 
					LCD_DisplayStringLine(Line5,string);
					sprintf((char*)string,"Threshold 3:%2dcm",high);
					LCD_DisplayStringLine(Line7,string);  
					}
				  else if(shezhi==1)
					{
				      shezhi=0;
						  qie=0;
							Delay_Ms(5);
							IIC_Write(0x00,low);
							Delay_Ms(5);
						  IIC_Write(0x01,mid);
							Delay_Ms(5);
						  IIC_Write(0x02,high);
				     	LCD_SetTextColor(White);	
				  	 	LCD_DisplayStringLine(Line1,(unsigned char*)" Liquid Level       ");
		         AD_Read();
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
		   if(qie==0|qie==3)
		   {
		   		LCD_SetBackColor(Blue);
	        LCD_SetTextColor(White);
					sprintf((char*)string,"Threshold 2:%2dcm",mid); 
					LCD_DisplayStringLine(Line5,string);
					sprintf((char*)string,"Threshold 3:%2dcm",high);
					LCD_DisplayStringLine(Line7,string);
					LCD_SetTextColor(Red);
          sprintf((char*)string,"Threshold 1:%2dcm",low);
				  LCD_DisplayStringLine(Line3,string);
			    qie=1;
		   }
		   else if (qie==1){
		   	  LCD_SetBackColor(Blue);
	        LCD_SetTextColor(White);
			
          sprintf((char*)string,"Threshold 1:%2dcm",low);
					LCD_DisplayStringLine(Line3,string);
					sprintf((char*)string,"Threshold 3:%2dcm",high);
					LCD_DisplayStringLine(Line7,string);
	
					LCD_SetTextColor(Red);
	       	sprintf((char*)string,"Threshold 2:%2dcm",mid); 
					LCD_DisplayStringLine(Line5,string);
				
			    qie=2;
		   
		   }
		   else if (qie==2)
			 {
		       LCD_SetBackColor(Blue);
	         LCD_SetTextColor(White);
			
           sprintf((char*)string,"Threshold 1:%2dcm",low);
					 LCD_DisplayStringLine(Line3,string);
					 sprintf((char*)string,"Threshold 2:%2dcm",mid); 
					 LCD_DisplayStringLine(Line5,string);
	
					 LCD_SetTextColor(Red);
				   sprintf((char*)string,"Threshold 3:%2dcm",high);
					 LCD_DisplayStringLine(Line7,string);
				
			    qie=3;
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
		  	 if(qie==1)
				 {
					 low=low+5;
					 if(low>=95)low=95;
					 sprintf((char*)string,"Threshold 1:%2dcm",low);
           LCD_DisplayStringLine(Line3,string);
			 	}
			 else if(qie==2)
			 {
					 mid=mid+5;
					 if(mid>=95)mid=95;
					 sprintf((char*)string,"Threshold 2:%2dcm",mid);
           LCD_DisplayStringLine(Line5,string);
			 }		  		  
			 else if(qie==3)
			 {
					 high=high+5;
					 if(high>=95)high=95;
					 sprintf((char*)string,"Threshold 3:%2dcm",high);
           LCD_DisplayStringLine(Line7,string);
			 }		  
		  }
		 while(!RB3);
   }

    if(RB4==0)
   {
   	  Delay_Ms(10);
		  if(RB4==0)
   		  {		
		  	 if(qie==1)
				 {
					 low=low-5;
					 if(low<=5)low=5;
					 sprintf((char*)string,"Threshold 1:%2dcm",low);
           LCD_DisplayStringLine(Line3,string);
			   }
			  else if(qie==2)
				{
					 mid=mid-5;
					 if(mid<=5)mid=5;
					 sprintf((char*)string,"Threshold 2:%2dcm",mid);
           LCD_DisplayStringLine(Line5,string);
			  }		  		  
			 else  if(qie==3)
			 {
					 high=high-5;
					 if(high<=5)high=5;
					 sprintf((char*)string,"Threshold 3:%2dcm",high);
           LCD_DisplayStringLine(Line7,string);
			 }
		  }
		 while(!RB4);
   }
}

void LED2_Run(void){
    int i;
		for(i=0;i<5;i++)
		{

			  liang=Bit_SET;
		  	GPIO_SetBits(GPIOD,GPIO_Pin_2);
		    GPIO_SetBits(GPIOC,LED1|LED3|LED4|LED5|LED6|LED7|LED7|LED8);
			  GPIO_ResetBits(GPIOC,LED2);
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);
		    Delay_Ms(200);
				GPIO_SetBits(GPIOD,GPIO_Pin_2);
				GPIO_SetBits(GPIOC,LED1|LED3|LED4|LED5|LED6|LED7|LED7|LED8);
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

	 		liang=Bit_SET;
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
    	GPIO_SetBits(GPIOC,LED1|LED2|LED4|LED5|LED6|LED7|LED7|LED8);
			GPIO_ResetBits(GPIOC,LED3);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
   	  Delay_Ms(200);
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
			GPIO_SetBits(GPIOC,LED1|LED2|LED4|LED5|LED6|LED7|LED7|LED8);
    	GPIO_SetBits(GPIOC,LED3);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	 		Delay_Ms(200);
	   }

}
void AD_Read(void)
{

   float ADC_VALUE; 
   ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
   ADC_VALUE = ADC_GetConversionValue(ADC1)*3.3/0xFFF;
   sprintf((char*)string,"%s%.2fV        ","ADC:",ADC_VALUE);
   LCD_DisplayStringLine(Line5,string);
   ADC_VALUE = ADC_VALUE*30.303;
   sprintf((char*)string,"%s%.2fcm        ","Height:",ADC_VALUE);
   LCD_DisplayStringLine(Line3,string);

   ADC_VAL= ADC_VALUE;
   if(ADC_VALUE>high)
	 {
   	  LCD_DisplayStringLine(Line7,(unsigned char*)"Level: 3                ");
			if (LEVEL==2)
			{
			   sprintf((char*)TX_buf,"A:H%2d+L3+U\r\n",ADC_VAL);
			   USART_Send(TX_buf);
			   LED2_Run();
	   	
		  }
		  LEVEL=3;
   }
	 else if(ADC_VALUE>mid)
	 {
   		LCD_DisplayStringLine(Line7,(unsigned char*)"Level: 2                ");
		   if (LEVEL==1)
			 {
		   sprintf((char*)TX_buf,"A:H%2d+L2+U\r\n",ADC_VAL);
		   USART_Send(TX_buf);
		   LED2_Run();
			 }
			else if(LEVEL==3)
			{
		   sprintf((char*)TX_buf,"A:H%2d+L2+D\r\n",ADC_VAL);
		   USART_Send(TX_buf);
		   LED2_Run();		
			}
	   	LEVEL=2;
   }
	 else if(ADC_VALUE>low)
	 {
	    LCD_DisplayStringLine(Line7,(unsigned char*)"Level: 1                ");
			if (LEVEL==0)
				{
			   sprintf((char*)TX_buf,"A:H%2d+L1+U\r\n",ADC_VAL);
			   USART_Send(TX_buf);
			   LED2_Run();
				}
			else if(LEVEL==2)
				{
			   sprintf((char*)TX_buf,"A:H%2d+L1+D\r\n",ADC_VAL);
			   USART_Send(TX_buf);
			   LED2_Run();
				}
			 LEVEL=1;
   }
	 else if(ADC_VALUE<low)
	 {
	     	LCD_DisplayStringLine(Line7,(unsigned char*)"Level: 0                 ");
			 if(LEVEL==1)
			 {
			   sprintf((char*)TX_buf,"A:H%2d+L0+D\r\n",ADC_VAL);
			   USART_Send(TX_buf);
		     LED2_Run();
		     LEVEL=0;
	     }
   }
}


void GPIO_Int(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		ADC_InitTypeDef  ADC_InitStructure;
		USART_InitTypeDef USART_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 

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



	/******************************************ADC INIT***********************************************************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_13Cycles5);
 
    ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));

    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

	/******************************************USART INIT***********************************************************/
    GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
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

    NVIC_Configuration();

	/******************************************LED Config***********************************************************/
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
    GPIO_SetBits(GPIOC,LEDALL);
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
  
  
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

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
void LCD_Init(void)
{
	  STM3210B_LCD_Init();
		LCD_Clear(Blue);
		LCD_SetBackColor(Blue);
		LCD_SetTextColor(White);
	
		LCD_DisplayStringLine(Line0,"                    ");	
		LCD_DisplayStringLine(Line1," Liquid Level       ");	
		LCD_DisplayStringLine(Line2,"                    ");
		LCD_DisplayStringLine(Line3," Height:            ");
		LCD_DisplayStringLine(Line4,"                    ");						
		LCD_DisplayStringLine(Line5," ADC:               ");	
		LCD_DisplayStringLine(Line6,"                    ");	
		LCD_DisplayStringLine(Line7,"Level:              ");		
		LCD_DisplayStringLine(Line8,"                    ");		
		LCD_DisplayStringLine(Line9,"                    ");

}

void Delay_Ms(u32 nTime)
{
		TimingDelay = nTime;
		while(TimingDelay != 0);	
}

