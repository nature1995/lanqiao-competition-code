#include "stm32f10x_it.h"
#include "pbdata.h"

void SysTick_Handler(void)
{
		LedTiming ++;
		TimingDelay --;
	
		if(LedTiming >= 1000)                                                        LedTiming  = 0;		
		if(KeyScanContuol == 1)  if(KeyScan() != 0 && KeyScan() != PlatformControl)  KeyControl = 1;			
		if(PlatformMove != 0)    if(--PlatformMove == 0)
		{	
				PlatformControl = MoveTemp;
				PlatformTempControl = 1;
				TwinkleTemp = 1;				
				if(TwinkleControl == 0)		Platform_Twinkle();				
		}

		if(KeyControl == 1) 		 if(++KeyTiming >= 1000)
		{
				KeyControl = 0;
				KeyTiming = 0;
				KeyScanContuol = 0;	
				GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		}
		
		if(--PlatformTiming == 0)
		{
				GPIO_ResetBits(GPIOA,GPIO_Pin_5);
				Pwm2Control = 499*0.5;
				KeyScanContuol = 0;
		}
}

void EXTI0_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line0) == SET)
		{
				EXTI_ClearITPendingBit(EXTI_Line0);
				TimeDisplayControl = 1;
				if(LedControl != 1 && TwinkleControl == 1 && PlatformControl != KeyScan()) KeyScanContuol = 1;
		}			
}

void EXTI9_5_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line8) == SET)
		{
				EXTI_ClearITPendingBit(EXTI_Line8);
				TimeDisplayControl = 1;
				if(LedControl != 1 && TwinkleControl == 1 && PlatformControl != KeyScan()) KeyScanContuol = 1;
		}			
}

void EXTI1_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line1) == SET)
		{
				EXTI_ClearITPendingBit(EXTI_Line1);
				TimeDisplayControl = 1;
				if(LedControl != 1 && TwinkleControl == 1 && PlatformControl != KeyScan()) KeyScanContuol = 1;
		}	
}

void EXTI2_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line2) == SET)
		{
				EXTI_ClearITPendingBit(EXTI_Line2);	
				TimeDisplayControl = 1;
				if(LedControl != 1 && TwinkleControl == 1 && PlatformControl != KeyScan()) KeyScanContuol = 1;
		}	
}

void RTC_IRQHandler(void)
{
		if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
		{			
				RTC_ClearITPendingBit(RTC_IT_SEC);
				RTC_WaitForLastTask();
				TimeDisplayControl = 1;

				if (RTC_GetCounter() == 0x0001517F)
				{
						RTC_SetCounter(0x00);
						RTC_WaitForLastTask();
				}
		}
}
