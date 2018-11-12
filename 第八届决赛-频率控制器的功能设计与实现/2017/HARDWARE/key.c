#include "key.h"
#include "led.h"
#include "i2c.h"
u8 window_key=1;
u8 window_option=1;
u32 fenpin=1,beipin=1;
void KEY_Init()
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
  
  /* Configure PA.00 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* Connect EXTI0 Line to PA.00 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
  EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_Init(&EXTI_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_Init(&EXTI_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_Init(&EXTI_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_Init(&NVIC_InitStructure);
}


void EXTI0_IRQHandler(void)
{
	delay_ms(10);
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    LED_Control(LED1,1);
		if(window_key==1)
		{
			window_key=2;
		}
		else if(window_key==2)
		{
			window_key=1;
			x24c02_write(0x01,(u8)fenpin);
			delay_ms(2);
			x24c02_write(0x02,(u8)beipin);
			delay_ms(2);
		}
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
    LED_Control(LED2,1);
		if(window_key==2)
		{
			window_option%=2;
			window_option++;
		}
		
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
}

void EXTI1_IRQHandler(void)
{
	delay_ms(10);
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    LED_Control(LED3,1);
		if(window_key==2)
		{
			if(window_option==1)
			{
				if(fenpin<4)fenpin++;
			}
			else if(window_option==2)
			{
				if(beipin<4)beipin++;
			}
		}
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

void EXTI2_IRQHandler(void)
{
	delay_ms(10);
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
    LED_Control(LED4,1);
		if(window_key==2)
		{
			if(window_option==1)
			{
				if(fenpin>1)fenpin--;
			}
			else if(window_option==2)
			{
				if(beipin>1)beipin--;
			}
		}
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}