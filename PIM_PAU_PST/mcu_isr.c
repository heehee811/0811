/*
 * mcu_isr.c
 *
 * Created: 2022-07-17 오전 2:46:20
 *  Author: jjh83
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#include <ADC.h>

#include <board.h>
#include <Encoder.h>
#include <CLCD.h>

//Module Button
ISR(INT4_vect)
{
	Clcd_Init();
	Set_DebugMode();
}
//Encoder Button
ISR(INT5_vect)
{
	Set_EncoderButton();
}
//Encoder Knob  
ISR(INT6_vect)
{
	if(PINE & _BV(ENCODER_Pin_B))
	{
		Increase_Encoder();
	}
	else
	{
		Decrease_Encoder();
	}
}

ISR(TIMER2_OVF_vect)
{
	Start_ADC();
}