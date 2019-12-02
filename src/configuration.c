#include "stm32f10x.h"
#include "configuration.h"
#include "keyboard.h"

#define BUTTON_DELAY_MS 350
uint8_t button_lock = 0;

inline void enable_button_lock()
{
		button_lock = 1;
		TIM_SetCounter(TIM3, 0);
		TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		button_lock = 0;
		TIM_SetCounter(TIM3, 0);
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		TIM_Cmd(TIM3, DISABLE);
	}
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) && !button_lock)
	{
		uint8_t pin_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
		GPIO_WriteBit(GPIOB, GPIO_Pin_5,!pin_state);
		handle_pressed_key(COLUMN_ABC);
		enable_button_lock();
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) && !button_lock)
	{
		uint8_t pin_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
		GPIO_WriteBit(GPIOB, GPIO_Pin_5,!pin_state);
		handle_pressed_key(COLUMN_369);
		enable_button_lock();
	}
		EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) && !button_lock)
	{
		uint8_t pin_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
		GPIO_WriteBit(GPIOB, GPIO_Pin_5,!pin_state);
		handle_pressed_key(COLUMN_258);
		enable_button_lock();
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) && !button_lock)
	{
		uint8_t pin_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
		GPIO_WriteBit(GPIOB, GPIO_Pin_5,!pin_state);
		handle_pressed_key(COLUMN_147);
		enable_button_lock();
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void enable_clocks(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

void configure_timers(void)
{
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);

	TIM_TimeBaseInitTypeDef tim_micro;
	TIM_TimeBaseStructInit(&tim_micro);
	tim_micro.TIM_CounterMode = TIM_CounterMode_Down;
	tim_micro.TIM_Prescaler = 72 -1;
	TIM_TimeBaseInit(TIM1,&tim_micro);

	TIM_TimeBaseInitTypeDef tim_mili;
	TIM_TimeBaseStructInit(&tim_mili);
	tim_mili.TIM_CounterMode = TIM_CounterMode_Down;
	tim_mili.TIM_Prescaler = 36000 -1;
	TIM_TimeBaseInit(TIM2,&tim_mili);

	TIM_TimeBaseInitTypeDef tim_button_handler;
	TIM_TimeBaseStructInit(&tim_button_handler);
	tim_button_handler.TIM_CounterMode = TIM_CounterMode_Up;
	tim_button_handler.TIM_Prescaler = 36000 - 1;
	TIM_TimeBaseInit(TIM3, &tim_button_handler);

	TIM_OCInitTypeDef button_outputcompare;
	TIM_OCStructInit(&button_outputcompare);
	button_outputcompare.TIM_OCMode = TIM_OCMode_Active;
	button_outputcompare.TIM_Pulse = BUTTON_DELAY_MS;
	TIM_OC1Init(TIM3, &button_outputcompare);
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

	// Keyboard GPIO configuration
	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = ROW_PIN_FIRST | ROW_PIN_SECOND |
			ROW_PIN_THIRD | ROW_PIN_FORTH;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEYBOARD_GPIO_PORT, &gpio);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_IPD;
	gpio.GPIO_Pin = COLUMN_PIN_FIRST | COLUMN_PIN_SECOND |
			COLUMN_PIN_THIRD | COLUMN_PIN_FORTH;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(KEYBOARD_GPIO_PORT, &gpio);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
}

void configure_spi(void)
{
	SPI_InitTypeDef spi;

	SPI_StructInit(&spi);
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_Init(SPI2, &spi);

	SPI_Cmd(SPI2, ENABLE);

	GPIO_InitTypeDef gpio;
//SPI2 PINS
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_15; // SCK & MOSI
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &gpio);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_14; // MISO
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &gpio);

//Radio CE Pin
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &gpio);

//SPI2 CHIP SELECT
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_8;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &gpio);

	GPIO_WriteBit(GPIOB, GPIO_Pin_8, ENABLE);
}

void configure_interrupts(void)
{
	EXTI_InitTypeDef ext_int;

	EXTI_StructInit(&ext_int);
	ext_int.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3;
	ext_int.EXTI_Mode = EXTI_Mode_Interrupt;
	ext_int.EXTI_Trigger = EXTI_Trigger_Rising;
	ext_int.EXTI_LineCmd = ENABLE;
	EXTI_Init(&ext_int);

	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);
}

void configure_nvic(void)
{
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannel = EXTI0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannel = EXTI1_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannel = EXTI2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannel = EXTI3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x04;
	nvic.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_Init(&nvic);
}


void configure_for_transmitter(void)
{
	enable_clocks();
	configure_timers();
	configure_i2c();
	configure_spi();
	configure_gpio();
	configure_interrupts();
	configure_nvic();
}

void configure_for_receiver(void)
{
	enable_clocks();
	configure_timers();
	configure_spi();
	configure_gpio();

	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, ENABLE);
}


