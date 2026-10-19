/*
 * ADS7886.c
 *
 * Created: 2022-07-18 오후 1:59:50
 *  Author: user
 */ 
#include <ADS7886.h>

uint16_t Get_ADS7886(gpio_pin_t en)
{
	uint8_t buf[2];
	uint16_t ret;

	Spi_Set_Dataorder(SPI_MSB_First);
	Spi_Set_Mode(SPI_Mode2);
	
	Set_gpio_Pin_Output_Low(en);
	Spi_Read_Data(buf,2);
	Set_gpio_Pin_Output_High(en);
	
	ret = buf[0];
	ret <<= 8;
	ret |= buf[1];

	return ret;
}