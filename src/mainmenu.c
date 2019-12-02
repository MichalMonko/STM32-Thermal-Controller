#include "mainmenu.h"
#include "lcd.h"

handler_type handler;

uint8_t current_character = 0;
uint8_t desired_temp_str[3] = {'-', '-', '-'} ;
uint8_t current_temp_str[3] =  {'-', '-', '-'};
uint16_t current_temperature = 0;



void set_current_temp(uint16_t temp)
{
	uint8_t temp_lower = temp | 0xff;
	uint8_t temp_upper = (temp >>8) | 0xff;
	current_temp_str[0] = 'A';
	menu_switch_state(DISPLAY_STATE);
	//TODO: PARSE 16 bits into temperature string
}

void _handle_button_display(uint8_t button)
{
	if(button == 'A')
		menu_switch_state(EDITION_STATE);
}

void menu_reset_second_row()
{
	lcd_move_cursor(1,0);

	for(int i =0; i < 15; i++)
	{
		lcd_write_char(ASCII_SPACE);
	}
	current_character = 0;
	lcd_move_cursor(1,0);
}

void menu_setup_second_row_for_writing()
{
	lcd_move_cursor(1,DESIRED_TEMP_COL);

	for(int i =0; i < MAX_TEMP_CHARS; i++)
	{
		lcd_write_char(ASCII_SPACE);
	}
	current_character = 0;
	lcd_move_cursor(1,DESIRED_TEMP_COL);
}

void _handle_button_edit(uint8_t button)
{
	if((ASCII_ZERO <= button) && (button <= ASCII_NINE))
	{
		if(current_character < MAX_TEMP_CHARS)
		{
			lcd_write_char(button);
			desired_temp_str[current_character++] = button;
		}
	}
	else if (button == 'B')
	{
		if(current_character > 0)
		{
			lcd_move_cursor(1,DESIRED_TEMP_COL + (--current_character));
			desired_temp_str[current_character] = ASCII_NULL;
			lcd_write_char('-');
			lcd_move_cursor(1,DESIRED_TEMP_COL + current_character);
		}
	}
	else if(button == 'C')
	{
		menu_setup_second_row_for_writing();
	}
	else if (button == 'A')
		menu_switch_state(DISPLAY_STATE);
}

void menu_switch_state(uint8_t state)
{
	menu_reset_second_row();
	if(state == DISPLAY_STATE)
	{
		lcd_blink_off();
		menu_switch_handler(_handle_button_display);
		menu_write_temperature();

	for(int i =0; i < MAX_TEMP_CHARS; i++)
	{
		desired_temp_str[i] = ASCII_NULL;
	}

	}
	else if(state == EDITION_STATE)
	{
		lcd_move_cursor(1,0);
		menu_write_temperature(desired_temp_str);
		current_character = 0;
		lcd_move_cursor(1, DESIRED_TEMP_COL);
		lcd_blink_on();
		menu_switch_handler(_handle_button_edit);
	}
}

void _menu_write_temp_value(uint8_t * value)
{
	uint8_t null_or_zero = 1;
	for(int i =0; i < MAX_TEMP_CHARS; i++)
	{
		if(value[i] != ASCII_NULL)
		{
			if(value[i] == '0' && null_or_zero)
				continue;
			lcd_write_char(value[i]);
			null_or_zero = 0;
		}
	}
	if(null_or_zero)
	{
		lcd_write_string("---", MAX_TEMP_CHARS);
	}
}

void menu_write_temperature()
{
	_menu_write_temp_value(current_temp_str);
	lcd_write_string(" / ", 3);
	_menu_write_temp_value(desired_temp_str);
}

void menu_init()
{
	lcd_move_cursor(0, 0);
	const char msg[] = "Obecna/Zadana:";
	lcd_write_string(msg,sizeof(msg) - 1);
	menu_switch_state(DISPLAY_STATE);
}

void menu_handle_button_pressed(uint8_t button)
{
	(*handler)(button);
}

void menu_switch_handler(handler_type new_func)
{
	handler = new_func;
}

int16_t parse_desired_temp()
{
	uint8_t ascii_zero = 0x30;
	int16_t returned_temp = -1;
	for(int i =2,multiplier=1; i >= 0; i--,multiplier*=10)
	{
		uint8_t temp_char = desired_temp_str[i];
		if((temp_char > 30) && (temp_char < 58))
		returned_temp += multiplier * temp_char;
		else
			break;
	}
	return returned_temp;
}
