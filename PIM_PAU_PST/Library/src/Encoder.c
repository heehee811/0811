/*
 * Encoder.c
 *
 * Created: 2022-07-18 오후 4:30:41
 *  Author: user
 */ 
#include <Encoder.h>

volatile int16_t Encoder_Value = 0;
volatile uint8_t Encoder_Button = false;

 void Increase_Encoder()
 {
	 ++Encoder_Value;
 }
 void Decrease_Encoder()
 {
	 --Encoder_Value;
 }
 void Clear_Encoder()
 {
	 Encoder_Value = 0;
 }
 int16_t Get_Encoder()
 {
	 return Encoder_Value;
 }

 void Set_EncoderButton()
 {
    Encoder_Button = true;
 }
 
 void Clr_EncoderButton()
 {
	 Encoder_Button = false;
 }


uint8_t Get_EncoderButton()
{
  return Encoder_Button;
}