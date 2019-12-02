#ifndef __RADIO__H__
#define __RADIO__H__
#include "stm32f10x.h"

typedef unsigned char uint8_t;

#define ADDRESS_LENGTH_BYTES 3

#define COMMAND_R_REGISTER 0x00
#define COMMAND_W_REGISTER 0x20
#define COMMAND_R_RX_PAYLOAD 0x61 //Use only in RX mode
#define COMMAND_W_TX_PAYLOAD 0xA0 //Use only in TX mode
#define COMMAND_W_ACK_PAYLOAD 0xa8 // Last 3 bits indicate pipe number
#define COMMAND_FLUSH_TX 0xE1
#define COMMAND_FLUSH_RX 0xE2
#define COMMAND_R_RX_PAYLOAD_WIDTH 0x60
#define COMMAND_NOP 0xff //Used to read status register

#define CONFIG_REGISTER_DEFAULT 0x08
#define CONFIG_REGISTER 0x00
#define CONFIG_PRIMARY_RX 0x01
#define CONFIG_PWR_UP 0x02
#define CONFIG_CRCO_2BYTES 0x04
#define CONFIG_ENABLE_CRC 0x08
#define CONFIG_MASK_MAX_RT_INT 0x10
#define CONFIG_MASK_TX_INT 0x20
#define CONFIG_MASK_RX_DR_INT 0x40

#define RF_CHANNEL_DEFAULT 0x02
#define RF_CHANNEL_REGISTER 0x05
#define RF_CHANNEL_VALUE 0xff

#define AIR_REGISTER_DEFAULT 0x0e
#define AIR_REGISTER 0x06
#define AIR_DATA_RATE_250kbps 0x10
#define AIR_DATA_RATE_1Mbps 0x00
#define AIR_DATA_RATE_2Mbps 0x01

#define STATUS_REGISTER 0x07
#define STATUS_RX_DR_BIT 0x40
#define STATUS_TX_DS_BIT 0x20
#define STATUS_MAX_RT_BIT 0x10
#define RX_PO_N_BITS 0x0d
#define TX_FULL_BIT 0x01

#define AUTO_RETRANSMIT_DEFAULT 0x03
#define AUTO_RETRANSMIT_REGISTER 0x04
#define DISABLE_AUT_RETRANSMIT 0x00

#define FIFO_STATUS_REGISTER 0x17
#define FIFO_STATUS_TX_REUSE_BIT 0x40
#define FIFO_STATUS_TX_FULL_BIT 0x20
#define FIFO_STATUS_TX_EMPTY_BIT 0x10
#define FIFO_STATUS_RX_FULL_BIT 0x02
#define FIFO_STATUS_RX_EMPTY_BIT 0x01

#define EN_AA_REGISTER 0x01
#define EN_AA_DEFAULT 0x3f
#define EN_AA_P0_BIT 0x01
#define EN_AA_P1_BIT 0x02
#define EN_AA_P2_BIT 0x04
#define EN_AA_P3_BIT 0x08
#define EN_AA_P4_BIT 0x10
#define EN_AA_P5_BIT 0x20

#define EN_RXADDR_REGISTER 0x02
#define EN_RXADDR_ENABLE_P0 0x01
#define EN_RXADDR_ENABLE_P1 0x02

#define ADDRESS_WIDTH_REGISTER 0x03
#define ADDRESS_WIDTH_3_BYTES 0x01
#define ADDRESS_WIDTH_4_BYTES 0x02
#define ADDRESS_WIDTH_5_BYTES 0x03

#define RX_ADDRESS_P0_REGISTER 0x0A
#define TX_ADDRESS_REGISTER 0x10
#define RX_PAYLOAD_WIDTH_P0_REGISTER 0x11

#define FEATURE_REGISTER 0x1d
#define FEATURE_EN_ACK_PAYLOAD_BIT 0x02
#define FEATURE_DYNAMIC_PAYLOAD_LENGHT_BIT 0x04

#define SETUP_RETR_REGISTER 0x04
#define RETRANSMIT_5_TIMES 0x05
#define RETRANSMIT_DELAY_1MS 0x30

#define DYNPD_REGISTER 0x1c
#define DYNPD_PIPE_0 0x01

#define PAYLOAD_WIDTH 0x02

void configure_chip_enable(GPIO_TypeDef * port, uint16_t pin);

void configure_as_transmitter(void);
void configure_as_receiver(void);

void switch_to_receiver();
void switch_to_transmitter();

void chip_enable_up();
void chip_enable_down();

void write_to_register(uint8_t reg, uint8_t data);
void write_to_register_multiple_bytes(uint8_t reg, uint8_t * data, uint8_t count);
void write_ack_payload(uint8_t * buffer, uint8_t how_many);
uint8_t send_command(uint8_t command);

uint8_t read_from_register(uint8_t reg);
void read_from_register_multiple_bytes(uint8_t reg, uint8_t * buffer, uint8_t count);

uint8_t last_status(void);
uint8_t force_read_status(void);
uint8_t read_fifo_status(void);

uint8_t read_payload_width();

uint8_t has_received_data();
uint8_t is_data_sent();
uint8_t has_sending_failed();

void clear_int_flags();
void flush_tx();
void flush_rx();

void read_received(uint8_t * buffer, uint8_t how_many);
void send(uint8_t * buffer, uint8_t how_many);

#endif
