#include "stm32f10x.h"
#include "i2c.h"

unsigned char error_status;

unsigned char i2c_check_errors(void) {
	if (I2C_GetFlagStatus(I2C1, I2C_FLAG_BERR))
		return BUS_ERROR_CODE;
	if (I2C_GetFlagStatus(I2C1, I2C_FLAG_AF))
		return ACK_FAIL_CODE;
	if (I2C_GetFlagStatus(I2C1, I2C_FLAG_ARLO))
		return ARBITRATION_LOST_CODE;

	return NO_ERROR_CODE;
}

unsigned char i2c_send_byte(const unsigned char base_address, unsigned char data) {
	I2C_GenerateSTART(I2C1, ENABLE);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS) {
		error_status = i2c_check_errors();
		if (error_status != NO_ERROR_CODE)
			return error_status;
	}

	I2C_Send7bitAddress(I2C1, base_address, I2C_Direction_Transmitter);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)
			!= SUCCESS) {
		error_status = i2c_check_errors();
		if (error_status != NO_ERROR_CODE)
			return error_status;
	}

	I2C_SendData(I2C1, data);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)
			!= SUCCESS) {
		error_status = i2c_check_errors();
		if (error_status != NO_ERROR_CODE)
			return error_status;
	}

	I2C_GenerateSTOP(I2C1, ENABLE);

	return NO_ERROR_CODE;
}

unsigned char i2c_send_data(const unsigned char base_address, unsigned char * data, unsigned char num) {
	I2C_GenerateSTART(I2C1, ENABLE);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS) {
		error_status = i2c_check_errors();
		if (error_status != NO_ERROR_CODE)
			return error_status;
	}

	I2C_Send7bitAddress(I2C1, base_address, I2C_Direction_Transmitter);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)
			!= SUCCESS) {
		error_status = i2c_check_errors();
		if (error_status != NO_ERROR_CODE)
			return error_status;
	}

	for (int i = 0; i < num; ++i) {
		I2C_SendData(I2C1, *data);
		while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)
				!= SUCCESS) {
			error_status = i2c_check_errors();
			if (error_status != NO_ERROR_CODE)
				return error_status;
		}
		data++;
	}

	I2C_GenerateSTOP(I2C1, ENABLE);

	return NO_ERROR_CODE;
}

//uint8_t read_data(const uint8_t base_register, uint8_t * data, uint num) {
//	uint8_t temp_byte;
//
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS) {
//		error_status = i2c_check_errors();
//		if (error_status != NO_ERROR_CODE)
//			return error_status;
//	}
//
//	I2C_Send7bitAddress(I2C1, I2C_Direction_Receiver);
//	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)
//			!= SUCCESS) {
//		error_status = i2c_check_errors();
//		if (error_status != NO_ERROR_CODE)
//			return error_status;
//	}
//
//	for (int i = 0; i < num; ++i) {
//		*data = I2C_ReceiveData(I2C1);
//		data++;
//		while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)
//				!= SUCCESS) {
//			error_status = i2c_check_errors();
//			if (error_status != NO_ERROR_CODE)
//				return error_status;
//		}
//	}
//
//	I2C_GenerateSTOP(I2C1, ENABLE);
//
//	return NO_ERROR_CODE;
//}
