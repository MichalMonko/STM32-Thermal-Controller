#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "stm32f10x.h"

#define ROWS 4
#define COLUMNS 4

#define KEYBOARD_GPIO_PORT GPIOA

#define ROW_PIN_FIRST GPIO_Pin_7
#define ROW_PIN_SECOND GPIO_Pin_6
#define ROW_PIN_THIRD GPIO_Pin_5
#define ROW_PIN_FORTH GPIO_Pin_4

#define COLUMN_PIN_FIRST GPIO_Pin_3
#define COLUMN_PIN_SECOND GPIO_Pin_2
#define COLUMN_PIN_THIRD GPIO_Pin_1
#define COLUMN_PIN_FORTH GPIO_Pin_0

#define COLUMN_ABC 3
#define COLUMN_369 2
#define COLUMN_258 1
#define COLUMN_147 0

#define ROW_0 3
#define ROW_789 2
#define ROW_456 1
#define ROW_123 0

const uint16_t columns_pins[COLUMNS];
const uint16_t rows_pins[ROWS];

const uint8_t button_map[ROWS][COLUMNS];

void keyboard_init(void);
void keyboard_reset_rows(uint8_t state);

inline void handle_pressed_key(uint8_t column_index);

#endif
