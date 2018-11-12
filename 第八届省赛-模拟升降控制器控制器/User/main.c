/*************************************************************************************
* 文件名称：第八届蓝桥杯省赛题——模拟升降控制器																			 *
* 实验目的：实现CT117E嵌入式竞赛板的LCD显示、按键功能、PWM输出、LED流水指示、RTC功能 *
* 程序说明：配置TIM1 CH1、TIM3 CH3为PWM输出模式，TIM3输出1KHZ，占空比80%或60%信号		 *
*          	TIM1输出2KHz，占空比60%或50%信号																				 *
* 软件环境：Keil uVision4 MDK4.50                                                    *
* 硬件环境：CT117E嵌入式竞赛板/Rev1.1                                                *
* 作品来源：电子创新工作室——Ruby20zh60                                               *
* 日期版本：2018-3-23																							          				 *
*************************************************************************************/
#include "pbdata.h"

int main(void)
{
		SystemInit();
		PBDATA_Configuration();
	
		STM3210B_LCD_Init();
		RTC_Configuration();
		LED_Configuration();
	  KEY_Configuration();
		PWM_Configuration();

		LCD_Clear(Blue2);
		LCD_SetBackColor(Blue2);
		LCD_SetTextColor(White);
		Delay_Ms(10);
		LCD_DisplayStringLine(Line2 ,"  Current Platform  ");
		LED_Control(LEDALL,0);
		PWM_Control(Pwm1Control ,Pwm2Control);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	
		while(1)
		{		
				TIME_Display();		
				Platform_Display();
				Platform_Move();	
				LED_Display();
				LED_CycleControl(LedControl);	
				KEY_Control(KeyControl);
				PWM_OutputControl();
		}
}
