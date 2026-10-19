/*
 * BVA.c
 *
 * Created: 2022-07-17 오전 1:56:45
 *  Author: jjh83
 */ 
#include <BVA.h>

void Bva_Set_ATT(gpio_pin_t en, uint8_t Address, uint8_t ATT)
{
	uint8_t buf[2];
    
	Spi_Set_Dataorder(SPI_LSB_First);
	Spi_Set_Mode(SPI_Mode0);
	
	ATT *= ATT_1dB;
	
	if(ATT > ATT_MAX)
	{
		ATT = ATT_MAX;
	}
	
	buf[0] = ATT;
	buf[1] = Address;
	
	Set_gpio_Output_Low(en.gpio_struct, en.pinnumber);
	
	Spi_Send_Data(buf,2);
	
	Set_gpio_Output_High(en.gpio_struct, en.pinnumber);
}