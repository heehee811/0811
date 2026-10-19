/*
 * systick.c
 *
 * Created: 2022-07-19 오전 11:32:20
 *  Author: user
 */ 
#include <systick.h>

uint32_t systick;

ISR(TIMER0_COMP_vect)
{
	TIFR |= _BV(OCF0);
	TCNT0 = 0;
	Increase_Systick();
}

void Init_Systick()
{
	systick = 0;
	//TOP Value for 1ms
	OCR0 = 173;
	//CTC & 64 prescaler
	TCCR0 = _BV(WGM01) | _BV(CS02);
	//Compare Match Interrupt Enable
	TIMSK |= _BV(OCIE0);
}

uint32_t Get_Systick()
{
	return systick;
}

void Increase_Systick()
{
	++systick;
}