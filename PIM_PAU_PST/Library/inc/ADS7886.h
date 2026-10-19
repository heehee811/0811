/*
 * ADS7886.h
 *
 * Created: 2022-07-17 오전 1:50:17
 *  Author: jjh83
 */ 


#ifndef ADS7886_H_
#define ADS7886_H_

#include <Spi_Device.h>
#include <gpio.h>

#define ADS7886_AREF	3300

uint16_t Get_ADS7886(gpio_pin_t en);



#endif /* ADS7886_H_ */