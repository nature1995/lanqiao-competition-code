#include "pwm.h"

float Pwm1Temp = 999*0.8;
float Pwm2Temp = 499*0.5;
float Pwm1Control = 999*0.8;
float Pwm2Control = 499*0.5;


void PWM_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	
		//PA4~PA5
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//PA6~PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE);   //TIM1≤ø∑÷”≥…‰	
}

void PWM_Control(float Channel1Pulse, float Channel2Pulse)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef        TIM_OCInitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	
		TIM_TimeBaseStructure.TIM_Prescaler = 0;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	
		/*TIM3*/
		TIM_TimeBaseStructure.TIM_Period = 999;  //1KHz
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_PrescalerConfig(TIM3,71, TIM_PSCReloadMode_Immediate);	
	
		/*TIM1*/
		TIM_TimeBaseStructure.TIM_Period = 499;  //2KHz
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
		TIM_PrescalerConfig(TIM1,71, TIM_PSCReloadMode_Immediate);	

		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	
		
		/*TIM3_CH1*/
		TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);

		/*TIM1_CH1*/
		TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);

		/*TIM3*/
		TIM_Cmd(TIM3, ENABLE);
		TIM_CtrlPWMOutputs(TIM3, ENABLE);
		
		/*TIM1*/
		TIM_Cmd(TIM1, ENABLE);
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void PWM_OutputControl(void)
{
		if(Pwm1Temp != Pwm1Control  || Pwm2Temp != Pwm2Control)
		{
				Pwm1Temp = Pwm1Control;
				Pwm2Temp = Pwm2Control;
				PWM_Control(Pwm1Temp,Pwm2Temp);
		}
}
