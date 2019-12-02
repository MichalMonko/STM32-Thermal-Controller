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

#define TRANSMITTER 0

int16_t desired_temp;
uint8_t desired_buffer[2] = {0,0};
uint8_t actual_buffer[2] = {0,0};
uint8_t buffer[3];

void parse_temp(uint8_t resolution);

void loop_transmitter()
{
	flush_tx();
	flush_rx();
	clear_int_flags();
	read_from_register_multiple_bytes(TX_ADDRESS_REGISTER, buffer,3);
	read_from_register_multiple_bytes(RX_ADDRESS_P0_REGISTER, buffer,3);
	while(1)
	{
		volatile uint8_t status;
		volatile uint8_t fifo_status;
		status = force_read_status();
//		desired_temp = parse_desired_temp();
		desired_temp = 84;
		desired_buffer[0] = (desired_temp & 0xff);
		desired_buffer[1] = (desired_temp >> 8) & 0xff;
		send(desired_buffer, 2);
		delay_ms(1000);
		status = force_read_status();
		if(is_data_sent())
		{
			read_received(actual_buffer, 2);
			set_current_temp(255);
		}
		flush_tx();
		flush_rx();
		clear_int_flags();
		fifo_status = read_fifo_status();
	}
}

void loop_receiver()
{
//	actual_buffer[0] = 0x42;
//	actual_buffer[1] = 0x42;
//	flush_tx();
//	flush_rx();
//	clear_int_flags();
//	read_from_register_multiple_bytes(TX_ADDRESS_REGISTER, buffer,3);
//	read_from_register_multiple_bytes(RX_ADDRESS_P0_REGISTER, buffer,3);
//	while(1)
//	{
//		write_ack_payload(actual_buffer,2);
//		volatile uint8_t status;
//		volatile uint8_t fifo_status;
//		chip_enable_up();
//		delay_ms(100);
//		chip_enable_down();
//		read_received(desired_buffer, 2);
//		status = force_read_status();
//		fifo_status = read_fifo_status();
//		flush_rx();
//		clear_int_flags();
//	}

	uint8_t conversion_resolution = CONVERSION_RESOLUTION_9_BIT;
	one_wire_set_pin(GPIOB, GPIO_Pin_9);

	while(ds18b20_configure(ALARM_LOW, ALARM_HIGH, conversion_resolution) != 0)
	{
		delay_ms(10);
	}

	uint8_t * ds18b20_buffer;
	volatile uint8_t temp_lsb;
	volatile uint8_t temp_msb;
	while(1)
	{
		ds18b20_convert_temp();
		ds18b20_read_temp_into_buffer();
		ds18b20_buffer = ds18b20_get_buffer();
		temp_lsb = ds18b20_buffer[TEMP_LSB_INDEX];
		temp_msb = ds18b20_buffer[TEMP_MSB_INDEX];
		parse_temp(conversion_resolution);
	}
}

void parse_temp(uint8_t resolution)
{

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
