/*
 * systick.h
 *
 * Created: 2022-07-19 오전 11:32:10
 *  Author: user
 */ 


#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void Init_Systick();
uint32_t Get_Systick();
void Increase_Systick();

#endif /* SYSTICK_H_ */