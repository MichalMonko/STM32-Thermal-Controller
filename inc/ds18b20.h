#ifndef __DS18B20_H__
#define __DF18B20_H__

#include "stm32f10x.h"

#define ONE_WIRE_COMMAND_INDEX 0x00
#define ONE_WIRE_FUNCTION_INDEX 0x01

#define ONE_WIRE_COMMAND_SEARCH_ROM 0xF0
#define ONE_WIRE_COMMAND_READ_ROM 0x33
#define ONE_WIRE_COMMAND_MATCH_ROM 0x55
#define ONE_WIRE_COMMAND_SKIP_ROM 0xCC
#define ONE_WIRE_COMMAND_ALARM_SEARCH 0xEC

#define ONE_WIRE_FUNCTION_CONVERT_TEMP 0x44
#define ONE_WIRE_FUNCTION_WRITE_SCRATCHPAD 0x4E
#define ONE_WIRE_FUNCTION_READ_SCRATCHPAD 0xBE
#define ONE_WIRE_FUNCTION_COPY_SCRATCHPAD 0x48
#define ONE_WIRE_FUNCTION_RECALL_E2 0xB8
#define ONE_WIRE_FUNCTION_READ_POWER_SUPPLY 0xB4

#define ALARM_LOW 0x80
#define ALARM_HIGH 0x7f

#define CONVERSION_RESOLUTION_12_BIT 0x7f
#define CONVERSION_RESOLUTION_11_BIT 0x5f
#define CONVERSION_RESOLUTION_10_BIT 0x3f
#define CONVERSION_RESOLUTION_9_BIT 0x1f

#define ONE_WIRE_ALARM_HIGH_INDEX 0x00
#define ONE_WIRE_ALARM_LOW_INDEX 0x01
#define ONE_WIRE_CONFIG_INDEX 0x02

#define EEPROM_WRITE_DELAY_MS 12

#define SCRACHTPAD_FULL 5
#define SCRATCHPAD_TEMP_ONLY 2

#define TEMP_LSB_INDEX 0x00
#define TEMP_MSB_INDEX 0x01

uint8_t ds18b20_configure(uint8_t alarm_low, uint8_t alarm_high, uint8_t resolution);

void ds18b20_read_rom();
void ds18b20_read_scrachtpad(uint8_t count);

void ds18b20_write_scrachpad();
void ds18b20_eeprom_copy();

void ds18b20_read_temp_into_buffer();

uint8_t ds18b20_verify_scrachpad();

void ds18b20_convert_temp();

uint8_t * ds18b20_get_buffer();

#endif
