#include "pwm.h"
#include "capture.h"
#include "key.h"

uint16_t capture = 0;
__IO uint16_t CCR1Val=100;
__IO uint16_t CCR2Val=1000;
void pwm_Init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable TIM9 and GPIOA clock  */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
  /* GPIOA Configuration:TIM9 Channel1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	

  /* Enable the TIM9 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =  TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 71;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Output Compare Toggle Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Toggle Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2Val;

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);

  /* TIM IT enable */
  TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
}

void TIM3_IRQHandler(void)
{
  /* TIM9_CH1 toggling with frequency = 366.2 Hz */
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
    capture = TIM_GetCapture1(TIM3);
    TIM_SetCompare1(TIM3, capture + CCR1Val );
  }

  /* TIM9_CH2 toggling with frequency = 732.4 Hz */
  if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    capture = TIM_GetCapture2(TIM3);
    TIM_SetCompare2(TIM3, capture + CCR2Val);
  }
}