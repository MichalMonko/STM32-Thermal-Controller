
#ifndef __ONE_WIRE_H__
#define __ONE_WIRE_H__

#include "stm32f10x.h"

#define RESET_PULSE_TIME 500
#define PRESENCE_CHECK_DELAY 80

#define WRITE_ONE_SLOT_HOLD_TIME 5
#define WRITE_SLOT_WINDOW 65
#define WRITE_SLOTS_DELAY 3

#define READ_SLOT_INIT_LOW_TIME 2
#define READ_SLOT_SAMPLE_DELAY 8
#define READ_SLOT_TOTAL_LENGTH 65
#define READ_SLOTS_DELAY 3

void one_wire_set_pin(GPIO_InitTypeDef * port, uint16_t pin);

uint8_t one_wire_reset_pulse();
void one_wire_send(uint8_t * buffer, uint8_t count );
void one_wire_send_one();
void one_wire_send_zero();

void one_wire_read(uint8_t * buffer, uint8_t count );
uint8_t one_wire_read_bit();

void one_wire_wait_till_complete();

#endif
