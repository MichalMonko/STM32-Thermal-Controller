#define DUMMY_VALUE 0xff;

#ifndef __SPI_DRIVER_H__
#define __SPI_DRIVER_H__
#include "stm32f10x.h"
#endif

typedef unsigned char uint8_t;

uint8_t spi_send_receive(uint8_t byte);
void configure_cs(GPIO_TypeDef * port, uint16_t pin);
