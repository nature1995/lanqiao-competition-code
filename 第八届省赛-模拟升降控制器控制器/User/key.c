#include "key.h"

uint8_t KeyOne = 0;
uint8_t KeyTwo = 0;
uint8_t KeyThree = 0;
uint8_t KeyFour = 0;
uint8_t KeyControl = 0;
uint8_t Key1Control = 49;
uint8_t Key2Control = 50;
uint8_t Key3Control = 51;
uint8_t Key4Control = 52;
uint8_t KeyScanContuol = 1;

uint32_t KeyTiming = 0;

void KEY_Configuration(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
		EXTI_InitTypeDef   EXTI_InitStructure;
		NVIC_InitTypeDef   NVIC_InitStructure;
	
		//GPIO_Configure
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
	
		//EXTI_Configure
		EXTI_ClearITPendingBit(EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line8);
	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource2);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
	
		EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line8;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		
		EXTI_Init(&EXTI_InitStructure);

		//NVIC_Configure	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

uint8_t KeyScan(void)
{
		uint8_t KeyTemp = 0;
	
		if(KEY1_IN == 0)   KeyTemp = '1';
		if(KEY2_IN == 0)	 KeyTemp = '2';
		if(KEY3_IN == 0)	 KeyTemp = '3';
		if(KEY4_IN == 0)	 KeyTemp = '4';
		return KeyTemp;
}

void KEY_Control(uint8_t KeyTemp)
{
		if(KeyTemp == 1)
		{	
				switch(KeyScan())
				{	
						case '1':
								Delay_Ms(10);
								KeyTiming = 0;
								if(PlatformControl != Key1Control) KeyOne = 1;
								while(!KEY1_IN);	
								break;	
						
						case '2':
								Delay_Ms(10);
								KeyTiming = 0;
								if(PlatformControl != Key2Control) KeyTwo = 1;
								while(!KEY2_IN);	
								break;		
						
						case '3':
								Delay_Ms(10);
								KeyTiming = 0;
								if(PlatformControl != Key3Control) KeyThree = 1;
								while(!KEY3_IN);	
								break;			
						
						case '4':
								Delay_Ms(10);
								KeyTiming = 0;
								if(PlatformControl != Key4Control) KeyFour = 1;
								while(!KEY4_IN);	
								break;
						
						default:   break;
				}
		}
}
