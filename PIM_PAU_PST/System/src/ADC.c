/*
 * ADC.c
 *
 * Created: 2022-07-19 오전 8:08:53
 *  Author: user
 */ 
#include <ADC.h>

volatile uint16_t Buffer_ADC[8];
volatile ADC_Status_t Adc_Status;

ISR(ADC_vect)
{
	uint32_t Buffer = ADCL;
	Buffer |= (ADCH<<8);
	
	Buffer *= System_AREF;
	
	Buffer >>= 10;
	
	Buffer_ADC[Adc_Status.Channel] = (uint16_t)Buffer;
}

void Init_ADC()
{
	Adc_Status.Status = 0;
	Adc_Status.Channel = 0;
	
	ADMUX = 0;
	ADCSRA = (1<<ADEN) | (0<<ADPS2) | (0<<ADPS1) | (1<<ADPS0) | (1<<ADIF) | (1<<ADIE);
}

void Start_ADC()
{
	ADCSRA |= _BV(ADSC);
	Adc_Status.Status = ADC_State_Running;
}

void ADC_Set_Channel(uint8_t Channel)
{
	ADMUX &= 0xE0;
	Channel &= 0x1F;
	
	ADMUX |= Channel;
}

uint8_t Get_ADC_Status()
{
	return Adc_Status.Status;
}

int16_t Get_ADC_Result(uint8_t Channel)
{
	return (int16_t)Buffer_ADC[Channel];
}