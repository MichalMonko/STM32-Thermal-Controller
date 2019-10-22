#include "stm32f10x.h"
#include "configuration.h"

void enable_clocks(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}

void configure_timers(void)
{
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);

	TIM_TimeBaseInitTypeDef tim_micro;
	TIM_TimeBaseStructInit(&tim_micro);
	tim_micro.TIM_CounterMode = TIM_CounterMode_Down;
	tim_micro.TIM_Prescaler = 36 -1;
	TIM_TimeBaseInit(TIM1,&tim_micro);

	TIM_TimeBaseInitTypeDef tim_mili;
	TIM_TimeBaseStructInit(&tim_mili);
	tim_mili.TIM_CounterMode = TIM_CounterMode_Down;
	tim_mili.TIM_Prescaler = 36000 -1;
	TIM_TimeBaseInit(TIM2,&tim_mili);
}
void configure_i2c(void)
{
	I2C_InitTypeDef i2c;
	I2C_StructInit(&i2c);
	i2c.I2C_Mode = I2C_Mode_I2C;
	i2c.I2C_ClockSpeed = 100000;
	I2C_Init(I2C1, &i2c);
	I2C_Cmd(I2C1, ENABLE);
}
void configure_gpio(void)
{
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_5;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
}

void configure(void)
{
	enable_clocks();
	configure_timers();
	configure_i2c();
	configure_gpio();
}


