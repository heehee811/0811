/*
 * Spi_Device.h
 *
 * Created: 2022-07-17 오전 1:33:37
 *  Author: jjh83
 */ 


#ifndef SPI_DEVICE_H_
#define SPI_DEVICE_H_

#include <avr/io.h>
#include <gpio.h>

enum 
{
    SPI_LSB_First,
    SPI_MSB_First
};

enum
{
	SPI_Mode0 = 0,
	SPI_Mode1,
	SPI_Mode2,
	SPI_Mode3
};

void Init_SPI(gpio_t* gpio, uint8_t ss, uint8_t sck, uint8_t mosi, uint8_t miso);
void Spi_Set_Dataorder(uint8_t order);
void Spi_Set_Mode(uint8_t mode);

void Spi_Set_Setting(uint8_t order, uint8_t mode);

void Spi_Send_Data(uint8_t* data, uint8_t length);
void Spi_Read_Data(uint8_t* buffer, uint8_t length);

#endif /* SPI_DEVICE_H_ */