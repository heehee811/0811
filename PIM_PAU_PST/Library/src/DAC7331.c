/*
 * DAC7331.c
 *
 * Created: 2022-07-17 오후 10:47:29
 *  Author: jjh83
 */ 
#include <DAC7331.h>

void Set_DAC(gpio_pin_t en, uint16_t Data)
{
	uint8_t buf[2];

	Spi_Set_Dataorder(SPI_MSB_First);
	Spi_Set_Mode(SPI_Mode2);

	if(Data > DAC_MAX)
	{
		Data = DAC_MAX;
	}
	
	Data &= 0xFFF;
	
	Data <<= 2;
	
	buf[0] = (Data & 0xFF00) >> 8;
	buf[1] = Data & 0xFF;
	
	Set_gpio_Output_Low(en.gpio_struct, en.pinnumber);
		
	Spi_Send_Data(buf,2);
	
	Set_gpio_Output_High(en.gpio_struct, en.pinnumber);
}