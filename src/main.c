/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "configuration.h"
#include "lcd.h"
#include "timer.h"


int main(void)
{
	configure();
	uint8_t status = lcd_init();
	lcd_write_string("Hello ,world!", 12);
	lcd_move_cursor(1,3);
	lcd_write_string("This is very long line that is long it is",24);
	lcd_move_cursor(0,7);
	lcd_write_string("is it me you looking for?",15);

	while (1)
	{

		GPIO_WriteBit(GPIOB, GPIO_Pin_5,Bit_SET );
		delay_seconds(3);
		GPIO_WriteBit(GPIOB, GPIO_Pin_5,Bit_RESET);
		delay_seconds(6);
	}

}
