#include "led.h"

uint8_t LedCycle = 0;
uint8_t LedControl = 0;

uint32_t LedTiming = 0;

void LED_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    //LED_PC8~PC15
    GPIO_InitStructure.GPIO_Pin = LEDALL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //74HC573_PD2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void LED_CycleControl(uint8_t LedControlTemp)
{
		if(LedControlTemp != 0)
		{
				if(LedCycle == 0)
				{
						switch(LedTiming /250%10)
						{
								case 0:   LED_Control(LED8,0);  LED_Control(LED5,1);    break;	
								case 1:   LED_Control(LED5,0);  LED_Control(LED6,1);    break;	
								case 2:   LED_Control(LED6,0);  LED_Control(LED7,1);    break;	
								case 3:   LED_Control(LED7,0);  LED_Control(LED8,1);    break;	
								default:                                                break;
						}
				}

				else
				{
						switch(LedTiming /250%10)
						{
								case 0:   LED_Control(LED5,0);  LED_Control(LED8,1);    break;	
								case 1:   LED_Control(LED8,0);  LED_Control(LED7,1);    break;
								case 2:   LED_Control(LED7,0);  LED_Control(LED6,1);    break;	
								case 3:   LED_Control(LED6,0);  LED_Control(LED5,1);    break;	
								default:                                                break;
						}
				}
		}
}

void LED_Control(uint16_t Led,uint8_t LedTemp)
{
    if(LedTemp == 0)
		{
        GPIO_SetBits(GPIOC,Led);
        GPIO_SetBits(GPIOD,GPIO_Pin_2);
        GPIO_ResetBits(GPIOD,GPIO_Pin_2);
    }        
    else
    {
        GPIO_ResetBits(GPIOC,Led);
        GPIO_SetBits(GPIOD,GPIO_Pin_2);
        GPIO_ResetBits(GPIOD,GPIO_Pin_2);
    }
}

void LED_Display(void)
{
		LED_Control(LED1,KeyOne);
		LED_Control(LED2,KeyTwo);
		LED_Control(LED3,KeyThree);
		LED_Control(LED4,KeyFour);
}
