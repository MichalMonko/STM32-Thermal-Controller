#include "timer.h"
#include "stm32f10x.h"

#define MILISECONDS_MULTIPLER 2 //Due to limit of period with prescaler to half milisecond per tick

void delay_us(unsigned long us)
{
	TIM_SetCounter(TIM1, us);
	TIM_Cmd(TIM1,ENABLE);
	while(TIM_GetCounter(TIM1));
	TIM_Cmd(TIM1,DISABLE);
}

void delay_ms(unsigned long ms)
{
	TIM_SetCounter(TIM2, ms * MILISECONDS_MULTIPLER);
	TIM_Cmd(TIM2,ENABLE);
	while(TIM_GetCounter(TIM2));
	TIM_Cmd(TIM2,DISABLE);
}

void delay_seconds(unsigned long seconds)
{
	while(seconds)
	{
		delay_ms(1000);
		seconds--;
	}
}
