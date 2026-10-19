/*
 * ADC.h
 *
 * Created: 2022-07-19 오전 8:08:45
 *  Author: user
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <sysconf.h>

enum
{
	ADC_State_Idle = 0,
	ADC_State_Running,
	ADC_State_Wating
};

typedef struct ADC_Status
{
	uint8_t Status;
	uint8_t Channel;
	uint32_t wait_tick;
}ADC_Status_t;

void Init_ADC();
void ADC_Interrupt(uint8_t on);
void Start_ADC();
void ADC_Set_Channel(uint8_t Channel);
uint8_t Get_ADC_Status();
int16_t Get_ADC_Result(uint8_t Channel);

#endif /* ADC_H_ */