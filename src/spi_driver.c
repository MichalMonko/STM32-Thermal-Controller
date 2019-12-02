#include "spi_driver.h"
#include "stm32f10x.h"

GPIO_TypeDef * port;
uint16_t pin;
uint8_t cs_status = 0;

void spi_cs_down()
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, RESET);
}

void spi_cs_up()
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, ENABLE);
}

uint8_t spi_send_receive(uint8_t byte)
{
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2, byte);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI2);
}

void configure_cs(GPIO_TypeDef * _port, uint16_t _pin)
{
	port = _port;
	pin = _pin;
}
