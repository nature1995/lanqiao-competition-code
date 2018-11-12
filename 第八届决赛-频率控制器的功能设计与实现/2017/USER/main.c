#include "led.h"
#include "key.h"
#include "lcd.h"
#include "adc.h"
#include "capture.h"
#include "pwm.h"
#include "i2c.h"

u16 AO1,AO2;
static __INLINE uint32_t SysTick_Config1(uint32_t ticks)
{ 
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
                                                               
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  return (0);                                                  /* Function successful */
}

void Window()
{
	u8 str1[20];
	if(window_key==1)
	{
		TIM_Cmd(TIM3, ENABLE);
		LCD_ClearLine(Line1);
		sprintf(str1,"      ce liang");
		LCD_DisplayStringLine(Line1,str1);
		LCD_ClearLine(Line6);
		sprintf(str1," AO1: %.2f",(float)AO1/4096*3.3);
		LCD_DisplayStringLine(Line6,str1);
		LCD_ClearLine(Line7);
		sprintf(str1," AO2: %.2f",(float)AO2/4096*3.3);
		LCD_DisplayStringLine(Line7,str1);
		
		LCD_ClearLine(Line4);
		sprintf(str1," PULS1: %.2f KHz",(float)TIM3Freq1/1000.);
		CCR1Val=(u16)(1000000*fenpin/TIM3Freq1/2);
		LCD_DisplayStringLine(Line4,str1);
		LCD_ClearLine(Line5);
		sprintf(str1," PULS2: %.2f KHz",(float)TIM3Freq2/1000.);
		CCR2Val=(u16)(1000000/(TIM3Freq2*beipin)/2);
		LCD_DisplayStringLine(Line5,str1);
		
		sprintf(str1,"                 1");
		LCD_DisplayStringLine(Line9,str1);
	}
	else if (window_key==2)
	{
		TIM_Cmd(TIM3, DISABLE);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		LCD_ClearLine(Line1);
		sprintf(str1,"      pei zhi");
		LCD_DisplayStringLine(Line1,str1);
		
		LCD_ClearLine(Line4);
		if(window_option==1){
			LCD_SetTextColor(Green);
		}
		sprintf(str1," fenpin: %d",(u32)fenpin);
		LCD_DisplayStringLine(Line4,str1);
		LCD_SetTextColor(Blue);
		
		LCD_ClearLine(Line5);
		if(window_option==2){
			LCD_SetTextColor(Green);
		}	
		sprintf(str1," beipin: %d",(u32)beipin);
		LCD_DisplayStringLine(Line5,str1);
		
		LCD_SetTextColor(Blue);
		LCD_ClearLine(Line6);
		LCD_ClearLine(Line7);
		LCD_DisplayChar(Line9, 319-16*17, '2');
		
	}
	
	LED_Control(LED_ALL,0);
}
int main()
{
	u8 v=2;
	SysTick_Config1(SystemCoreClock/1000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	STM3210B_LCD_Init();
	delay_ms(500);
	LCD_Clear(White);
	LCD_SetBackColor(White);
	LCD_SetTextColor(Blue);
	LED_Init();
	KEY_Init();
	adc_Init();
	capture_Init();
	pwm_Init();
	i2c_init();
// 	x24c02_write(0x01,v);
// 	delay_ms(2);
	fenpin=(u32)x24c02_read(0x01);
	delay_ms(2);
// 	x24c02_write(0x02,2);
// 	delay_ms(2);
	beipin=(u32)x24c02_read(0x02);
	delay_ms(2);
	LED_Control(LED_ALL,0);
	while(1)
	{
		AO1=get_ADC(ADC_Channel_4);
		AO2=get_ADC(ADC_Channel_5);
		Window();
		delay_ms(1000);
	}
	
}