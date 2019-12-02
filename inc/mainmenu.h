#ifndef __MAINMENU_H__
#define __MAINMENU_H_

#include "stm32f10x.h"

typedef void (*handler_type)(uint8_t);

#define MAX_TEMP_CHARS 3
#define DESIRED_TEMP_COL 6

#define DISPLAY_STATE 0x00
#define EDITION_STATE 0x01

#define ASCII_ZERO 0x30
#define ASCII_NINE 0x39

#define ASCII_SPACE 0x20
#define ASCII_NULL 0x00

void menu_init();
void menu_reset_second_row();

int16_t parse_desired_temp();
void set_current_temp(uint16_t temp);
void menu_write_temperature();
void menu_switch_state(uint8_t state);
void menu_handle_button_pressed(uint8_t button);
void _handle_button_display(uint8_t button);
void _handle_button_edit(uint8_t button);

void menu_switch_handler(handler_type new_func);

#endif
