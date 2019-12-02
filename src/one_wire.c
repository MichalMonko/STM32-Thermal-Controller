#include "one_wire.h"
#include "timer.h"
#include "stm32f10x.h"

GPIO_TypeDef * gpio_port;
uint16_t gpio_pin;

void one_wire_set_pin(GPIO_InitTypeDef * port, uint16_t pin)
{
	gpio_port = port;
	gpio_pin = pin;
	GPIO_WriteBit(port,pin, ENABLE);
}

uint8_t one_wire_reset_pulse()
{
	uint8_t slave_response = 1;
	GPIO_WriteBit(gpio_port, gpio_pin, RESET);
	delay_us(RESET_PULSE_TIME);
	GPIO_WriteBit(gpio_port, gpio_pin,ENABLE);
	delay_us(PRESENCE_CHECK_DELAY);
	slave_response = GPIO_ReadInputDataBit(gpio_port, gpio_pin);
	delay_us(RESET_PULSE_TIME - PRESENCE_CHECK_DELAY);
	return ~slave_response; // True if slave pulls down 1 wire line (Presence signal)
}


void one_wire_send(uint8_t * buffer, uint8_t count )
{
	uint8_t byte;
	for(uint8_t byte_ind = 0; byte_ind < count; byte_ind++)
	{
		byte = buffer[byte_ind];
		for(uint8_t mask=0x01; mask != 0x00; mask <<=1)
		{
			if(byte & mask)
				one_wire_send_one();
			else
				one_wire_send_zero();
		}
	}
}

void one_wire_read(uint8_t * buffer, uint8_t count )
{
	uint8_t byte;
	uint8_t sampled_bit = 0;
	for(uint8_t byte_ind = 0; byte_ind < count; byte_ind++)
	{
		byte = 0;
		for(uint8_t mask=0x01; mask != 0x00; mask <<=1)
		{
			sampled_bit = one_wire_read_bit();
			if(sampled_bit)
			{
				byte |= mask;
			}
			delay_us(READ_SLOT_TOTAL_LENGTH - READ_SLOT_INIT_LOW_TIME -
					READ_SLOT_SAMPLE_DELAY + READ_SLOTS_DELAY);
		}
		buffer[byte_ind] = byte;
	}
}

void one_wire_send_one()
{
	GPIO_WriteBit(gpio_port, gpio_pin, RESET);
	delay_us(WRITE_ONE_SLOT_HOLD_TIME);
	GPIO_WriteBit(gpio_port, gpio_pin, ENABLE);
	delay_us(WRITE_SLOT_WINDOW - WRITE_ONE_SLOT_HOLD_TIME + WRITE_SLOTS_DELAY);
}

void one_wire_send_zero()
{
	GPIO_WriteBit(gpio_port, gpio_pin, RESET);
	delay_us(WRITE_SLOT_WINDOW);
	GPIO_WriteBit(gpio_port, gpio_pin, ENABLE);
	delay_us(WRITE_SLOTS_DELAY);
}

uint8_t one_wire_read_bit()
{
	GPIO_WriteBit(gpio_port,gpio_pin, RESET);
	delay_us(READ_SLOT_INIT_LOW_TIME);
	GPIO_WriteBit(gpio_port,gpio_pin, ENABLE);
	delay_us(READ_SLOT_SAMPLE_DELAY);
	return GPIO_ReadInputDataBit(gpio_port, gpio_pin);
}

void one_wire_wait_till_complete()
{
	while(one_wire_read_bit() != 0x01);
}
