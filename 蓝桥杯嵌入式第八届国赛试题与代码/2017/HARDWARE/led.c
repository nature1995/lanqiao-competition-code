#include "led.h"

void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED_ALL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
}

void LED_Control(u16 LED,u8 state)
{
	if(state==0)
	{
		GPIO_SetBits(GPIOC,LED);
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
		
	}
	else if(state==1)
	{
		GPIO_ResetBits(GPIOC,LED);
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	}
}