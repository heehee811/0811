/*
 * Encoder.h
 *
 * Created: 2022-07-18 오후 4:29:59
 *  Author: user
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint-gcc.h>
#include <stdbool.h>

void Increase_Encoder();
void Decrease_Encoder();
void Clear_Encoder();
int16_t Get_Encoder();

void Set_EncoderButton();
void Clr_EncoderButton();
uint8_t Get_EncoderButton();


#endif /* ENCODER_H_ */