/*
 * Display.h
 *
 * Created: 2022-07-18 오후 5:41:41
 *  Author: user
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint-gcc.h>
#include <stdbool.h>
#include <system.h>

#include <system.h>
#include <Packet.h>
#include <CLCD.h>
#include <Encoder.h>

//Display Flag
typedef union
{
	uint16_t All;
	struct
	{
		uint16_t Mode:3;            //Display Mode
		uint16_t Changed:1;         //Need Refresh
		uint16_t Selection:4;       //Selection (0~15)
		uint16_t LINE:2;
		uint16_t Flags:2;           //Flag
		uint16_t VChanged:1;        //Value Changed(High Speed)
	};
}PAU_DisplayFlag;

void Task_Display(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr);

void Display_Main(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr);
void Display_Settings(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr);
void Display_Alarm(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr);

#endif /* DISPLAY_H_ */