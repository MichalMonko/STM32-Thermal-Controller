#include "radio2401l.h"
#include "stm32f10x.h"
#include "spi_driver.h"
#include "timer.h"


uint8_t status;
GPIO_TypeDef * port;
uint16_t pin;


uint8_t base_address[3] = {0x80,0x80,0x80};

void configure_chip_enable(GPIO_TypeDef * _port, uint16_t _pin)
{
	port = _port;
	pin = _pin;
}

void chip_enable_up()
{
	GPIO_SetBits(port,pin);
}

void chip_enable_down()
{
	GPIO_ResetBits(port,pin);
}

void write_to_register(uint8_t reg, uint8_t data)
{
	spi_cs_down();
	spi_send_receive(COMMAND_W_REGISTER | reg);
	status = spi_send_receive(data);
	spi_cs_up();
}

void write_to_register_multiple_bytes(uint8_t reg, uint8_t * data, uint8_t count)
{
	spi_cs_down();
	spi_send_receive(COMMAND_W_REGISTER | reg);
	for(uint8_t i = 0; i < count; i++)
	{
	status = spi_send_receive(data[i]);
	}
	spi_cs_up();
}

uint8_t read_from_register(uint8_t reg)
{
	uint8_t reg_value;
	spi_cs_down();
	status = spi_send_receive(COMMAND_R_REGISTER | reg);
	reg_value = spi_send_receive(COMMAND_NOP);
	spi_cs_up();

	return reg_value;
}

uint8_t send_command(uint8_t command)
{
	spi_cs_down();
	status = spi_send_receive(command);
	uint8_t result = spi_send_receive(COMMAND_NOP);
	spi_cs_up();
	return result;
}

uint8_t last_status(void)
{
	return status;
}

uint8_t force_read_status()
{
	status = read_from_register(STATUS_REGISTER);
	return status;
}

uint8_t read_fifo_status()
{
	return read_from_register(FIFO_STATUS_REGISTER);
}

void configure_as_transmitter(void)
{
	uint8_t conf_reg_value = CONFIG_REGISTER_DEFAULT | CONFIG_ENABLE_CRC
			 &(~CONFIG_PRIMARY_RX) | CONFIG_PWR_UP;
	uint8_t air_data_rate_reg_value = 0x06 | AIR_DATA_RATE_1Mbps;

	write_to_register(CONFIG_REGISTER, conf_reg_value);
	write_to_register(ADDRESS_WIDTH_REGISTER, ADDRESS_WIDTH_3_BYTES);
	write_to_register_multiple_bytes(RX_ADDRESS_P0_REGISTER,base_address, sizeof(base_address));
	write_to_register_multiple_bytes(TX_ADDRESS_REGISTER,base_address, sizeof(base_address));
	write_to_register(EN_AA_REGISTER, EN_AA_P0_BIT);
	write_to_register(FEATURE_REGISTER, FEATURE_DYNAMIC_PAYLOAD_LENGHT_BIT |
			FEATURE_EN_ACK_PAYLOAD_BIT);
	write_to_register(DYNPD_REGISTER, DYNPD_PIPE_0);
	write_to_register(AIR_REGISTER, air_data_rate_reg_value);
	write_to_register(SETUP_RETR_REGISTER, RETRANSMIT_5_TIMES | RETRANSMIT_DELAY_1MS);
}

uint8_t has_received_data()
{
	return status & STATUS_RX_DR_BIT;
}
uint8_t is_data_sent()
{
	return status & STATUS_TX_DS_BIT;
}

uint8_t has_sending_failed()
{
	return status & STATUS_MAX_RT_BIT;
}

void clear_int_flags()
{
	force_read_status();
	uint8_t cleared_status = status | STATUS_MAX_RT_BIT | STATUS_RX_DR_BIT
			| STATUS_TX_DS_BIT;
	write_to_register(STATUS_REGISTER, cleared_status);
}

void flush_tx()
{
	send_command(COMMAND_FLUSH_TX);
}
void flush_rx()
{
	send_command(COMMAND_FLUSH_RX);
}

void read_received(uint8_t *  buffer, uint8_t how_many)
{
	spi_cs_down();
	status = spi_send_receive(COMMAND_R_RX_PAYLOAD);
	for(uint8_t i =0; i < how_many; i++)
	{
		buffer[i] = spi_send_receive(COMMAND_NOP);
	}
	spi_cs_up();
}

void send(uint8_t * buffer, uint8_t how_many)
{
	spi_cs_down();
	spi_send_receive(COMMAND_W_TX_PAYLOAD);
	for (uint8_t i =0 ; i < how_many; i++)
	{
		status = spi_send_receive(buffer[i]);
	}
	spi_cs_up();
	chip_enable_up();
	delay_us(15);
	chip_enable_down();
}

void write_ack_payload(uint8_t * buffer, uint8_t how_many)
{
	spi_cs_down();
	spi_send_receive(COMMAND_W_ACK_PAYLOAD);
	for (uint8_t i =0 ; i < how_many; i++)
	{
		status = spi_send_receive(buffer[i]);
	}
	spi_cs_up();
}


void configure_as_receiver(void)
{
	uint8_t conf_reg_value = CONFIG_REGISTER_DEFAULT | CONFIG_ENABLE_CRC
			| CONFIG_PRIMARY_RX | CONFIG_PWR_UP;
	uint8_t air_data_rate_reg_value = 0x06 | AIR_DATA_RATE_1Mbps;

	write_to_register(CONFIG_REGISTER, conf_reg_value);
	write_to_register(EN_RXADDR_REGISTER, EN_RXADDR_ENABLE_P0);
	write_to_register(EN_AA_REGISTER, EN_AA_P0_BIT);
	write_to_register(ADDRESS_WIDTH_REGISTER, ADDRESS_WIDTH_3_BYTES);
	write_to_register(RX_PAYLOAD_WIDTH_P0_REGISTER, PAYLOAD_WIDTH);
	write_to_register_multiple_bytes(RX_ADDRESS_P0_REGISTER,base_address, sizeof(base_address));
	write_to_register(AIR_REGISTER, air_data_rate_reg_value);
	write_to_register(FEATURE_REGISTER, FEATURE_DYNAMIC_PAYLOAD_LENGHT_BIT |
			FEATURE_EN_ACK_PAYLOAD_BIT);
	write_to_register(DYNPD_REGISTER, DYNPD_PIPE_0);
}

void switch_to_receiver()
{
	uint8_t config_reg_value = read_from_register(CONFIG_REGISTER);
	write_to_register(CONFIG_REGISTER, config_reg_value | CONFIG_PRIMARY_RX);
}

void switch_to_transmitter()
{
	uint8_t config_reg_value = read_from_register(CONFIG_REGISTER);
	write_to_register(CONFIG_REGISTER, config_reg_value  &(~CONFIG_PRIMARY_RX));
}

uint8_t read_payload_width()
{
	return send_command(COMMAND_R_RX_PAYLOAD_WIDTH);
}

void read_from_register_multiple_bytes(uint8_t reg, uint8_t * buffer, uint8_t count)
{
	spi_cs_down();
	status = spi_send_receive(COMMAND_R_REGISTER | reg);
	for(uint8_t i =0; i < count; i++)
	{
		buffer[i] = spi_send_receive(COMMAND_NOP);
	}
	spi_cs_up();
}
