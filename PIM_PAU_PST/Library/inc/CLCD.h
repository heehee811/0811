/*
 * CLCD.h
 *
 * Created: 2022-07-16 오후 7:59:08
 *  Author: jjh83
 */ 


#ifndef CLCD_H_
#define CLCD_H_

#include <util/delay.h>
#include <stdio.h>
#include <gpio.h>

#define CMD_DISPLAY_CLR       		0x01
#define CMD_DISPLAY_HOME			0x02
#define CMD_ENTRY_MODE_SET    		0x04
#define CMD_DISPLAY_CONTROL    		0x08
#define CMD_DISPLAY_SHIFT			0x10
#define CMD_8BIT_MODE         		0x38
//#define CMD_4BIT_MODE 			0x28

#define Shift_left					0
#define Shift_right					4
#define Shift_disp_left				8
#define Shift_disp_right			12

#define Entry_Left					2
#define Entry_right					0

#define D_Bit 4
#define C_Bit 2
#define B_Bit 1

#define LINE1						0x80
#define LINE2						0xC0
#define LINE3						0x94
#define LINE4						0xD4

void Clcd_Open(gpio_t * ctrl, gpio_t * data, uint8_t rs, uint8_t rw, uint8_t en);
void Clcd_Init();
int Clcd_Putchar(uint8_t ch,FILE * fp); 
void Clcd_Command(uint8_t command);
void Clcd_Data(uint8_t data);

void Clcd_mode_Instruction();
void Clcd_mode_Command();

void Clcd_mode_Write();
void Clcd_mode_Read();

void Clcd_Enable();
void Clcd_Disable();

#endif /* CLCD_H_ */