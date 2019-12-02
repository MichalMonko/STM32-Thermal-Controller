#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x.h"

#define LCD_BASE_ADDRESS 0x4E

// Instructions
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02

// Entry Mode Instructions
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_ENTRY_MODE_INCREMENT 0x02
#define LCD_ENTRY_MODE_DECREMENT 0x00
#define LCD_ENTRY_MODE_DISPLAY_SHIFT 0x01

//Display control
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_DISPLAY_ON 0x04
#define LCD_DISPLAY_CURSOR_ON 0x02
#define LCD_DISPLAY_BLINK_ON 0x01

//Cursor or display shift
#define LCD_SHIFT 0x10
#define LCD_SHIFT_DISPLAY_SHIFT 0x08
#define LCD_SHIFT_CURSOR_SHIFT 0x00
#define LCD_SHIFT_RIGHT 0x04
#define LCD_SHIFT_LEFT 0x00

//Function set
#define LCD_FUNCTION_SET 0x20
#define LCD_FUNCTION_SET_8BITS 0x10
#define LCD_FUNCTION_SET_4BITS 0x00
#define LCD_FUNCTION_SET_2LINES 0x08
#define LCD_FUNCTION_SET_1LINE 0x00
#define LCD_FUNCTION_SET_BIG_FONT 0x04
#define LCD_FUNCTION_SET_SMALL_FONT 0x00

//Cursor operation
#define LCD_FIRST_LINE 0x00
#define LCD_SECOND_LINE 0x40
#define LCD_ADDRESS_SET 0x80

//Bits
#define LCD_ENABLE_BIT 0x04
#define LCD_BACKLIGHT_BIT 0x08

//Initialization fig 24 in documentation
#define LCD_START_SEQUENCE 0x30

#define LCD_INSTRUCTION_MODE 0x00
#define LCD_WRITE_MODE 0x01

void lcd_backlilght_on();
void lcd_backlilght_off();
uint8_t lcd_init();

uint8_t send_half_byte(uint8_t value);

uint8_t  lcd_write_char(uint8_t character);

uint8_t lcd_instruction(uint8_t code);

uint8_t send_in_4bit_mode(uint8_t value, uint8_t mode);

uint8_t lcd_move_cursor(uint8_t row, uint8_t column);

uint8_t lcd_write_string(const char * string, int len);

uint8_t lcd_blink_on();

uint8_t lcd_blink_off();

void lcd_clear();

void lcd_home();

#endif

