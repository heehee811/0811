/*
 * BVA.h
 *
 * Created: 2022-07-17 오전 1:49:57
 *  Author: jjh83
 */ 


#ifndef BVA_H_
#define BVA_H_

#include <gpio.h>
#include <Spi_Device.h>

#define ATT_MAX     127     //7Bit
#define ATT_1dB     4       //0.25dB @ 1Step

void Bva_Set_ATT(gpio_pin_t en, uint8_t Address, uint8_t ATT);

#endif /* BVA_H_ */