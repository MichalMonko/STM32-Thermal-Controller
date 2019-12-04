/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "configuration.h"
#include "lcd.h"
#include "timer.h"
#include "keyboard.h"
#include "mainmenu.h"
#include "radio2401l.h"
#include "spi_driver.h"
#include "ds18b20.h"

#define TEMP_SENTINEL_VALUE 0xff
#define HISTERESIS 3

#define MAX_FAILURES 5

#define TRANSMITTER 1


uint8_t old_temp = TEMP_SENTINEL_VALUE;
int16_t desired_temp;
uint8_t desired_buffer[2] = {0,0};
uint8_t actual_buffer[2] = {0,0};
uint8_t buffer[3];

uint8_t received_desired_temp = TEMP_SENTINEL_VALUE;
uint8_t histeresis_enabled = 0;
uint8_t relay_status = 0;

uint8_t failure_counter = 0;

GPIO_TypeDef * relay_port = GPIOB;
uint16_t relay_pin = GPIO_Pin_7;

uint8_t parse_temp(uint8_t lsb, uint8_t msb);
void set_histeresis_and_relay_status(uint8_t temp);

void loop_transmitter()
{
	while(1)
	{
		desired_buffer[0] = parse_desired_temp();
		if(!is_temp_valid())
		{
			desired_buffer[0] = TEMP_SENTINEL_VALUE;
		}

		send(desired_buffer, 1);
		delay_ms(1000);
		force_read_status();
		if(is_data_sent() && has_received_data())
		{
			read_received(actual_buffer, 1);
			failure_counter = 0;
		}
		else
		{
			failure_counter++;
			if(++failure_counter > MAX_FAILURES)
			{
				actual_buffer[0] = TEMP_SENTINEL_VALUE;
			}
		}
		flush_tx();
		flush_rx();
		clear_int_flags();
		set_current_temp(actual_buffer[0]);
	}
}

void loop_receiver()
{
	actual_buffer[0] = TEMP_SENTINEL_VALUE;
	desired_buffer[0] = TEMP_SENTINEL_VALUE;

	uint8_t * ds18b20_buffer;
	uint8_t temp_lsb;
	uint8_t temp_msb;
	one_wire_set_pin(GPIOB, GPIO_Pin_9);
	while(ds18b20_configure(ALARM_LOW, ALARM_HIGH, CONVERSION_RESOLUTION_9_BIT) != 0)
	{
		delay_ms(10);
	}

	while(1)
	{
		write_ack_payload(actual_buffer,1);
		chip_enable_up();
		delay_ms(1000);
		chip_enable_down();
		force_read_status();
		if(has_received_data())
		{
			read_received(desired_buffer, 1);
			received_desired_temp = desired_buffer[0];
			if(received_desired_temp != old_temp)
			{
				histeresis_enabled = 0;
				old_temp = received_desired_temp;
			}
			failure_counter = 0;
		}
		else
		{
			if(++failure_counter >= MAX_FAILURES)
			{
				received_desired_temp = TEMP_SENTINEL_VALUE;
			}
		}
		flush_rx();
		flush_tx();
		clear_int_flags();

		ds18b20_convert_temp();
		ds18b20_read_temp_into_buffer();
		ds18b20_buffer = ds18b20_get_buffer();
		temp_lsb = ds18b20_buffer[TEMP_LSB_INDEX];
		temp_msb = ds18b20_buffer[TEMP_MSB_INDEX];
		actual_buffer[0] = parse_temp(temp_lsb,temp_msb);
		set_histeresis_and_relay_status(actual_buffer[0]);

		GPIO_WriteBit(relay_port, relay_pin, relay_status);
 }
}

void set_histeresis_and_relay_status(uint8_t temp)
{
	if(received_desired_temp == TEMP_SENTINEL_VALUE)
	{
		return;
	}

	if(temp < received_desired_temp)
	{
		if(histeresis_enabled)
		{
			if(temp < (received_desired_temp - HISTERESIS))
			{
				relay_status = 1;
				histeresis_enabled = 0;
			}
			else
			{
				relay_status = 0;
				histeresis_enabled = 1;
			}
		}
		else
		{
			relay_status = 1;
			histeresis_enabled = 0;
		}
	}
	else
	{
		relay_status = 0;
		histeresis_enabled = 1;
	}
}

uint8_t parse_temp(uint8_t lsb, uint8_t msb)
{
	if ((msb & (0xf0)) > 0)
	{
		//Minus values are not used in this application, indicate error
		return TEMP_SENTINEL_VALUE;
	}
	uint16_t rounding_reminder = lsb & ~(0x08);

	return (lsb >> 4) | ((msb & 0x07) << 4);
}



int main(void)
{
configure_cs(GPIOB, GPIO_Pin_8);
configure_chip_enable(GPIOB, GPIO_Pin_12);
if(TRANSMITTER == 1)
{
	configure_for_transmitter();
	configure_as_transmitter();
	keyboard_init();
	lcd_init();
	menu_init();
}
else
{
	configure_for_receiver();
	configure_as_receiver();
}

//	lcd_write_string("Hello ,world!", 12);
//	lcd_move_cursor(1,3);
//	lcd_write_string("This is very long line that is long it is",24);
//	lcd_move_cursor(0,7);
//	lcd_write_string("is it me you looking for?",15);

	if(TRANSMITTER == 1)
	{
		loop_transmitter();
	}
	else
	{
		loop_receiver();
	}

}
