/*
 * DAC7331.h
 *
 * Created: 2022-07-17 오전 1:50:31
 *  Author: jjh83
 */ 


#ifndef DAC7331_H_
#define DAC7331_H_

#include <gpio.h>
#include <Spi_Device.h>

#define DAC_MAX     4095    //12Bit

void Set_DAC(gpio_pin_t en, uint16_t Data);

#endif /* DAC7331_H_ */