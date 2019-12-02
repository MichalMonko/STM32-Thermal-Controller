#include "keyboard.h"
#include "mainmenu.h"
#include "i2c.h"
#include "timer.h"

const uint16_t columns_pins[COLUMNS] = {COLUMN_PIN_FIRST, COLUMN_PIN_SECOND,
		COLUMN_PIN_THIRD, COLUMN_PIN_FORTH };
const uint16_t rows_pins[ROWS] = {ROW_PIN_FIRST, ROW_PIN_SECOND,
		ROW_PIN_THIRD, ROW_PIN_FORTH };

const uint8_t button_map[ROWS][COLUMNS] = {
		{'1','2','3','A'},
		{'4','5','6','B'},
		{'7','8','9','C'},
		{'*','0','#','D'}
};

void keyboard_init()
{
	keyboard_reset_rows(ENABLE);
}

void keyboard_reset_rows(uint8_t state)
{
	GPIO_WriteBit(KEYBOARD_GPIO_PORT, ROW_PIN_FIRST, state);
	GPIO_WriteBit(KEYBOARD_GPIO_PORT, ROW_PIN_SECOND, state);
	GPIO_WriteBit(KEYBOARD_GPIO_PORT, ROW_PIN_THIRD, state);
	GPIO_WriteBit(KEYBOARD_GPIO_PORT, ROW_PIN_FORTH, state);
}

extern inline void handle_pressed_key(uint8_t column_index)
{
	uint16_t column_pin = columns_pins[column_index];
	uint16_t row_pin = 0;
	keyboard_reset_rows(DISABLE);

	uint8_t row_index;
	for(row_index = 0; row_index < ROWS; row_index++)
	{
		row_pin = rows_pins[row_index];
		GPIO_WriteBit(KEYBOARD_GPIO_PORT, row_pin, ENABLE);

		if(GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, column_pin) == ENABLE)
			break;
	}

	uint8_t character = button_map[row_index][column_index];
	menu_handle_button_pressed(character);
	delay_ms(25);
	keyboard_reset_rows(ENABLE);
}
