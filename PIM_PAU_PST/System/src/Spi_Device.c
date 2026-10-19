/*
 * Spi_Device.c
 *
 * Created: 2022-07-17 오전 2:12:35
 *  Author: jjh83
 */ 
#include <Spi_Device.h>

void Init_SPI(gpio_t* gpio, uint8_t ss, uint8_t sck, uint8_t mosi, uint8_t miso)
{
		Set_gpio_Direction_Input(gpio, ss);
		Set_gpio_Output_High(gpio, ss);
    Set_gpio_Direction_Output(gpio, sck);
    Set_gpio_Direction_Output(gpio, mosi);
    Set_gpio_Direction_Input(gpio, miso);

    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1);
    //SPSR = _BV(SPI2X);
}

void Spi_Set_Dataorder(uint8_t order)
{
  if(order == SPI_MSB_First)
  {
    SPCR &= ~_BV(DORD);
  }
  else
  {
    SPCR |= _BV(DORD);
  }
}

void Spi_Set_Mode(uint8_t mode)
{
	SPCR &= 0xF3;
	mode &= 0x03;
	
	mode <<= 2;
	
	SPCR |= mode;
}

void Spi_Set_Setting(uint8_t order, uint8_t mode)
{
	SPCR &= ~_BV(SPE);
	
	Spi_Set_Dataorder(order);
	Spi_Set_Mode(mode);
	
	SPCR |= _BV(SPE);
}

uint8_t Spi_Get_Busy()
{
  return SPSR & _BV(SPIF);
}

void Spi_Send_Data(uint8_t* data, uint8_t length)
{
	uint8_t jdata;
  for(uint8_t i=0;i<length;i++)
  {
		SPDR = data[i];
		while(!Spi_Get_Busy());
		SPSR |= _BV(SPIF);
		jdata = SPDR;
  }
}

void Spi_Read_Data(uint8_t* buffer, uint8_t length)
{	
    for(uint8_t i=0;i<length;i++)
    {
	    SPDR = 0x00;
      while(!Spi_Get_Busy());
			SPSR |= _BV(SPIF);
      buffer[i] = SPDR;
    }
}
