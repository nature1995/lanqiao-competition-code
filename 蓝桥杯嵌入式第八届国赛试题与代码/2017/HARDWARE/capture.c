#include "capture.h"

void capture_Init()
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1|GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 64;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	 TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(TIM2, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM2, TIM_IT_CC2|TIM_IT_CC3, ENABLE);
}

__IO uint16_t IC3ReadValue11 = 0, IC3ReadValue21 = 0;
__IO uint16_t CaptureNumber1 = 0;
__IO uint32_t Capture1 = 0;
__IO uint32_t TIM3Freq1 = 0;

__IO uint16_t IC3ReadValue12 = 0, IC3ReadValue22 = 0;
__IO uint16_t CaptureNumber2 = 0;
__IO uint32_t Capture2 = 0;
__IO uint32_t TIM3Freq2 = 0;

void TIM2_IRQHandler(void)
{ 
   if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET) 
   {
    if(CaptureNumber1 == 0)
    {
      /* Get the Input Capture value */
      IC3ReadValue11 = TIM_GetCapture2(TIM2);
      CaptureNumber1 = 1;
    }
    else if(CaptureNumber1 == 1)
    {
      /* Get the Input Capture value */
      IC3ReadValue21 = TIM_GetCapture2(TIM2); 
      
      /* Capture computation */
      if (IC3ReadValue21 > IC3ReadValue11)
      {
        Capture1 = (IC3ReadValue21 - IC3ReadValue11); 
      }
      else
      {
        Capture1 = ((0xFFFF - IC3ReadValue11) + IC3ReadValue21); 
      }
      /* Frequency computation */ 
      TIM3Freq1 = (uint32_t) SystemCoreClock /64/ Capture1;
      CaptureNumber1 = 0;
    }
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
   }
	
	if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET) 
  {
    if(CaptureNumber2 == 0)
    {
      /* Get the Input Capture value */
      IC3ReadValue12 = TIM_GetCapture3(TIM2);
      CaptureNumber2 = 1;
    }
    else if(CaptureNumber2 == 1)
    {
      /* Get the Input Capture value */
      IC3ReadValue22 = TIM_GetCapture3(TIM2); 
      
      /* Capture computation */
      if (IC3ReadValue22 > IC3ReadValue12)
      {
        Capture2 = (IC3ReadValue22 - IC3ReadValue12); 
      }
      else
      {
        Capture2 = ((0xFFFF - IC3ReadValue12) + IC3ReadValue22); 
      }
      /* Frequency computation */ 
      TIM3Freq2 = (uint32_t) SystemCoreClock /64/ Capture2;
      CaptureNumber2 = 0;
    }
		 TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
  }
	
}