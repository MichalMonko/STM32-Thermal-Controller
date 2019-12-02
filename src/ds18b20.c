#include "ds18b20.h"
#include "one_wire.h"

uint8_t buffer[9];
uint8_t scratchpad_buffer[3];

void ds18b20_read_rom()
{
	one_wire_reset_pulse();
	buffer[ONE_WIRE_COMMAND_INDEX] = ONE_WIRE_COMMAND_READ_ROM;
	one_wire_send(buffer,1);
	one_wire_read(buffer, 8);
}

void ds18b20_convert_temp()
{
	one_wire_reset_pulse();
	buffer[ONE_WIRE_COMMAND_INDEX] = ONE_WIRE_COMMAND_SKIP_ROM;
	buffer[ONE_WIRE_FUNCTION_INDEX] = ONE_WIRE_FUNCTION_CONVERT_TEMP;
	one_wire_send(buffer,2);
	one_wire_wait_till_complete();
}

void ds18b20_read_temp_into_buffer()
{
	ds18b20_read_scrachtpad(SCRATCHPAD_TEMP_ONLY);
}

void ds18b20_read_scrachtpad(uint8_t count)
{
	one_wire_reset_pulse();
	buffer[ONE_WIRE_COMMAND_INDEX] = ONE_WIRE_COMMAND_SKIP_ROM;
	buffer[ONE_WIRE_FUNCTION_INDEX] = ONE_WIRE_FUNCTION_READ_SCRATCHPAD;
	one_wire_send(buffer,2);
	one_wire_read(buffer, count);
}

uint8_t ds18b20_configure(uint8_t alarm_low, uint8_t alarm_high, uint8_t resolution)
{
	ds18b20_write_scrachpad();
	ds18b20_read_scrachtpad(SCRACHTPAD_FULL);
	uint8_t is_valid = ds18b20_verify_scrachpad();

	if(is_valid == 0)
	{
		ds18b20_eeprom_copy();
	}
	return is_valid;
}

void ds18b20_write_scrachpad()
{
	one_wire_reset_pulse();
	buffer[ONE_WIRE_COMMAND_INDEX] = ONE_WIRE_COMMAND_SKIP_ROM;
	buffer[ONE_WIRE_FUNCTION_INDEX] = ONE_WIRE_FUNCTION_WRITE_SCRATCHPAD;
	one_wire_send(buffer,2);
	scratchpad_buffer[ONE_WIRE_ALARM_LOW_INDEX] = ALARM_LOW;
	scratchpad_buffer[ONE_WIRE_ALARM_HIGH_INDEX] = ALARM_HIGH;
	scratchpad_buffer[ONE_WIRE_CONFIG_INDEX] = CONVERSION_RESOLUTION_9_BIT;
	one_wire_send(scratchpad_buffer, 3);
}

void ds18b20_eeprom_copy()
{
	one_wire_reset_pulse();
	buffer[ONE_WIRE_COMMAND_INDEX] = ONE_WIRE_COMMAND_SKIP_ROM;
	buffer[ONE_WIRE_FUNCTION_INDEX] = ONE_WIRE_FUNCTION_COPY_SCRATCHPAD;
	one_wire_send(buffer,2);
	delay_ms(EEPROM_WRITE_DELAY_MS);
}

uint8_t ds18b20_verify_scrachpad()
{
	if(
			((buffer[2] ^ scratchpad_buffer[0]) != 0)
			|| ((buffer[3] ^ scratchpad_buffer[1]) != 0)
			|| ((buffer[4] ^ scratchpad_buffer[2])) != 0)
		return 1;
	return 0;
}

uint8_t * ds18b20_get_buffer()
{
	return buffer;
}
