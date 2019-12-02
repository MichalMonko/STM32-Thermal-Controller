#include "i2c.h"
#include "lcd.h"
#include "timer.h"

uint8_t lcd_init() {
	delay_ms(50);
	i2c_send_byte(LCD_BASE_ADDRESS, 0x00);
	delay_ms(1000);

	send_half_byte(LCD_START_SEQUENCE);
	delay_ms(5);
	send_half_byte(LCD_START_SEQUENCE);
	delay_ms(5);
	send_half_byte(LCD_START_SEQUENCE);
	delay_us(120);
	send_half_byte(LCD_FUNCTION_SET);
	delay_us(120);

	lcd_instruction(
			LCD_FUNCTION_SET | LCD_FUNCTION_SET_4BITS | LCD_FUNCTION_SET_2LINES
					| LCD_FUNCTION_SET_SMALL_FONT);
	lcd_instruction(LCD_ENTRY_MODE_SET | LCD_ENTRY_MODE_INCREMENT);

	lcd_instruction(
			LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_DISPLAY_CURSOR_ON
					| LCD_DISPLAY_BLINK_ON);

	lcd_clear();
	lcd_home();
	lcd_backlight_on();
}

uint8_t lcd_write_string(const char * string, int len)
{
	for(int i =0; i < len; i++)
	{
		lcd_write_char(string[i]);
	}
}

uint8_t send_half_byte(uint8_t value) {
	i2c_send_byte(LCD_BASE_ADDRESS, value | LCD_ENABLE_BIT);
	delay_us(1);
	i2c_send_byte(LCD_BASE_ADDRESS, value & ~LCD_ENABLE_BIT);
	delay_us(40);
}

uint8_t lcd_blink_on()
{
	lcd_instruction(
			LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_DISPLAY_CURSOR_ON
					| LCD_DISPLAY_BLINK_ON);
}

uint8_t lcd_blink_off()
{
	lcd_instruction(
			LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
}

void lcd_backlight_on()
{
	i2c_send_byte(LCD_BASE_ADDRESS, LCD_BACKLIGHT_BIT);
}

void lcd_backlight_off()
{
	i2c_send_byte(LCD_BASE_ADDRESS, 0x00);
}

uint8_t lcd_move_cursor(uint8_t row, uint8_t column)
{
	uint8_t row_mask = (row == 0) ? LCD_FIRST_LINE : LCD_SECOND_LINE;
	lcd_instruction(LCD_ADDRESS_SET  | row_mask + column );
}

uint8_t lcd_write_char(uint8_t character) {
	send_in_4bit_mode(character, LCD_WRITE_MODE);
}

uint8_t lcd_instruction(uint8_t code) {
	send_in_4bit_mode(code, LCD_INSTRUCTION_MODE);
}

uint8_t send_in_4bit_mode(uint8_t value, uint8_t mode) {
	uint8_t msh = value & 0xf0;
	uint8_t lsh = (value << 4) & 0xf0;
	send_half_byte(msh | mode | LCD_BACKLIGHT_BIT);
	send_half_byte(lsh | mode | LCD_BACKLIGHT_BIT);
}

void lcd_clear() {
	lcd_instruction(LCD_CLEAR_DISPLAY);
	delay_ms(10);
}

void lcd_home() {
	lcd_instruction(LCD_RETURN_HOME);
	delay_ms(2);
}

