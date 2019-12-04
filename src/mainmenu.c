#include "mainmenu.h"
#include "lcd.h"

handler_type handler;
uint8_t current_state = DISPLAY_STATE;

uint8_t current_character = 0;
uint8_t desired_temp_str[3] = {'-', '-', '-'} ;
uint8_t current_temp_str[3] =  {'-', '-', '-'};
uint16_t current_temperature = 0;

uint8_t temp_valid = 0;

uint8_t ASCII_DIGITS[10] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
uint8_t DIGITS_MULTIPLIERS[4] = {0,1,10,100};


void set_current_temp(uint8_t temp)
{
	uint8_t original_temp = temp;

	current_temp_str[0] = '-';
	current_temp_str[1] = '-';
	current_temp_str[2] = '-';

	if(temp == 0xff)
	{
		current_temp_str[0] = 'E';
		current_temp_str[1] = 'R';
		current_temp_str[2] = 'R';
	}
	else
	{
		current_temp_str[0] = (temp -(temp % 100)) / 100;
		temp = temp - current_temp_str[0] * 100;
		current_temp_str[1] = (temp -(temp % 10)) / 10;
		temp = temp - current_temp_str[1] * 10;
		current_temp_str[2] = temp;

		if(original_temp >= 100)
		{
			current_temp_str[0] = ASCII_DIGITS[current_temp_str[0]];
			current_temp_str[1] = ASCII_DIGITS[current_temp_str[1]];
			current_temp_str[2] = ASCII_DIGITS[current_temp_str[2]];
		}
		else if(original_temp >= 10)
		{
			current_temp_str[0] = ASCII_SPACE;
			current_temp_str[1] = ASCII_DIGITS[current_temp_str[1]];
			current_temp_str[2] = ASCII_DIGITS[current_temp_str[2]];
		}
		else
		{
			current_temp_str[0] = ASCII_SPACE;
			current_temp_str[1] = ASCII_SPACE;
			current_temp_str[2] = ASCII_DIGITS[current_temp_str[2]];
		}
	}

	if(current_state != EDITION_STATE)
		menu_switch_state(DISPLAY_STATE);
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
	current_state = state;
	menu_reset_second_row();
	if(state == DISPLAY_STATE)
	{


		lcd_blink_off();
		menu_switch_handler(_handle_button_display);
		menu_write_temperature();

	}
	else if(state == EDITION_STATE)
	{
		for(int i =0; i < MAX_TEMP_CHARS; i++)
		{
			desired_temp_str[i] = ASCII_NULL;
		}

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

uint8_t parse_desired_temp()
{
	temp_valid = 0;
	uint16_t returned_temp = 0;

	uint8_t blank_counter = 0;
	for(int i =0; i < MAX_TEMP_CHARS; i++)
	{
		if((desired_temp_str[i] < ASCII_ZERO) || (desired_temp_str[i] > (ASCII_ZERO + 9)))
		{
			blank_counter++;
		}
	}
	uint8_t number_of_digits = MAX_TEMP_CHARS - blank_counter;

	for(uint8_t i =0, multiplier=DIGITS_MULTIPLIERS[number_of_digits];
			multiplier > 0; multiplier /= 10, i++
			)
	{
		returned_temp += (desired_temp_str[i] - ASCII_ZERO) * multiplier;
		temp_valid = 1;
	}

	return (uint8_t) returned_temp;
}

uint8_t is_temp_valid()
{
	return temp_valid;
}
