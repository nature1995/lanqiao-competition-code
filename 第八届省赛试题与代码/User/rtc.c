#include "rtc.h"

uint8_t TimeData[20];

void RTC_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;

		NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		PWR_BackupAccessCmd(ENABLE);
		BKP_DeInit();
		RCC_LSICmd(ENABLE);

		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		RTC_WaitForLastTask();
		RTC_SetPrescaler(40000);
		RTC_WaitForLastTask();
		
		RTC_SetCounter(HH*3600 + MM*60 + SS);
		RTC_WaitForLastTask();		
}

void TIME_Read(u32 TimeRead)
{
		u32 THH = 0, TMM = 0, TSS = 0;
	
		THH = TimeRead / 3600;
		TMM = (TimeRead % 3600) / 60;
		TSS = (TimeRead % 3600) % 60;

		sprintf((char *)TimeData,"      %0.2d:%0.2d:%0.2d", THH, TMM, TSS);
}

void TIME_Display(void)
{
		if(TimeDisplayControl == 1)
		{
				TimeDisplayControl = 0;
				TIME_Read(RTC_GetCounter());
				LCD_DisplayStringLine(Line8,TimeData);
		}
}
