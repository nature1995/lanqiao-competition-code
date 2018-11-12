#include "pbdata.h"

u32 TimingDelay = 0;

uint8_t MoveTemp = 49;
uint8_t TwinkleTemp = 1;
uint8_t TwinkleControl = 1;
uint8_t PlatformControl = 49;
uint8_t PlatformTemp = 32;		  
uint8_t PlatformTempControl = 1;	
uint8_t PlatformData[20];

uint32_t PlatformMove = 0;
uint32_t PlatformTiming = 0;
uint32_t PlatformTwinkle = 0;
uint32_t TimeDisplayControl = 1;

void Delay_Ms(u32 nTime)
{
		TimingDelay = nTime;
		while(TimingDelay != 0);	
}

void PBDATA_Configuration(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		SysTick_Config(SystemCoreClock/1000);

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

void Platform_Display(void)
{
		if(PlatformTempControl == 1) PlatformTemp = PlatformControl;
		sprintf((char *)PlatformData,"          %c",PlatformTemp);
		LCD_DisplayStringLine(Line5,PlatformData);
}

void Platform_Twinkle(void)
{
		PlatformMove = 1;
		LedControl = 0;
		PlatformTwinkle ++;

		switch(PlatformTwinkle /333%10)
		{
				case 0:   PlatformTempControl = 0;  PlatformTemp = 32;  break;
				case 1:   PlatformTempControl = 1;                      break;	
				case 2:   PlatformTempControl = 0;  PlatformTemp = 32;  break;	
				case 3:   PlatformTempControl = 1;                      break;				
				default:  PlatformMove = 0;  
									PlatformTwinkle = 0;  
									KeyScanContuol = 1;  
									GPIO_SetBits(GPIOA,GPIO_Pin_5);
									Pwm2Control = 499*0.6;
									TwinkleControl = 1;
									
				if(KeyOne==1 ||KeyTwo==1 || KeyThree==1 || KeyFour==1) 
					PlatformTiming = 2000;	else  PlatformTiming = 4000;  break;
		}  
}

void Platform_Move(void)
{
		if(KeyScanContuol == 0)
		{
				switch(PlatformControl)
				{
						case 49:
								if(((KeyTwo == 1) || (KeyThree == 1) || (KeyFour == 1)) && TwinkleControl == 1 && TwinkleTemp == 1 && KeyScanContuol == 0)
								{
										PlatformMove = 6000;
										LedControl = 1;
										LedCycle = 0;
										Pwm1Control = 999*0.8;
										GPIO_SetBits(GPIOA,GPIO_Pin_4);
										MoveTemp = Key2Control;
										TwinkleTemp = 0;
										if(KeyTwo == 0 && (KeyThree == 1 || KeyFour == 1))	TwinkleControl = 1; else TwinkleControl = 0;
										break;
								}	
								
						case 50: 
								if(((KeyThree == 1) || (KeyFour == 1)) && TwinkleControl == 1 && TwinkleTemp == 1 && KeyScanContuol == 0)
								{
										PlatformMove = 6000;
										LedControl = 1;
										LedCycle = 0;
										Pwm1Control = 999*0.8;
										GPIO_SetBits(GPIOA,GPIO_Pin_4);
										MoveTemp = Key3Control;
										TwinkleTemp = 0;
										if(KeyThree == 0 && KeyFour == 1)	TwinkleControl = 1; else TwinkleControl = 0;
										break;
								}
								else if(((KeyOne == 1)) && TwinkleControl == 1 && TwinkleTemp == 1 && KeyScanContuol == 0)
								{
										PlatformMove = 6000;
										LedControl = 1;
										LedCycle = 1;
										Pwm1Control = 999*0.6;
										GPIO_ResetBits(GPIOA,GPIO_Pin_4);
										MoveTemp = Key1Control;
										TwinkleTemp = 0;
										TwinkleControl = 0;
										break;
								}
							
						case 51: 
								if(((KeyFour == 1)) && TwinkleControl == 1 && TwinkleTemp == 1 && KeyScanContuol == 0)
								{
										PlatformMove = 6000;
										LedControl = 1;
										LedCycle = 0;
										Pwm1Control = 999*0.8;
										GPIO_SetBits(GPIOA,GPIO_Pin_4);
										MoveTemp = Key4Control;
										TwinkleTemp = 0;
										TwinkleControl = 0;
										break;
								}
								else if(((KeyOne == 1) || (KeyTwo == 1)) && TwinkleControl == 1 && TwinkleTemp == 1 && KeyScanContuol == 0)
								{
										PlatformMove = 6000;
										LedControl = 1;
										LedCycle = 1;
										Pwm1Control = 999*0.6;
										GPIO_ResetBits(GPIOA,GPIO_Pin_4);
										MoveTemp = Key2Control;
										TwinkleTemp = 0;
										if(KeyTwo == 0 && KeyOne == 1) TwinkleControl = 1; else TwinkleControl = 0;
										break;
								}
							
						case 52: 
								if(((KeyOne == 1) || (KeyTwo == 1) || (KeyThree == 1)) && TwinkleControl == 1 && TwinkleTemp == 1 && KeyScanContuol == 0)
								{
										PlatformMove = 6000;
										LedControl = 1;
										LedCycle = 1;
										Pwm1Control = 999*0.6;
										GPIO_ResetBits(GPIOA,GPIO_Pin_4);
										MoveTemp = Key3Control;
										TwinkleTemp = 0;
										if(KeyThree == 0 && (KeyOne == 1 || KeyTwo == 1))	TwinkleControl = 1; else TwinkleControl = 0;
										break;
								}
									
						default: break;
				}
		}
		
		if(LedControl == 0)  switch(PlatformControl)
		{
				case 49:   KeyOne = 0;     break;
				case 50:   KeyTwo = 0;     break;
				case 51:   KeyThree = 0;   break;
				case 52:   KeyFour = 0;    break;
				default:                   break;
		}
}
