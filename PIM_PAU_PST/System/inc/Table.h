/*
 * Table.h
 *
 * Created: 2022-07-26 오전 10:18:49
 *  Author: user
 */ 


#ifndef TABLE_H_
#define TABLE_H_

#include <avr/eeprom.h>

#include <stdint-gcc.h>
#include <stdbool.h>

#include <sysconf.h>

#define Detect_Table_Section __attribute__ ((section (".detec_table")))
#define Delta_Table_Section __attribute__ ((section (".delta_table")))

typedef struct PAU_Detect
{
	uint16_t Atten;
	uint16_t Detect;
}Pau_Detect_t;

typedef struct PAU_Detect_Table
{
	uint8_t Setted;
	Pau_Detect_t Table[Table_Length];
}PAU_Detect_Table_t;

typedef struct Pau_Delta
{
	uint16_t Freq;
	int8_t Delta;
}Pau_Delta_t;

typedef struct Pau_Delta_Table
{
	Pau_Delta_t Origin;
	Pau_Delta_t Tables[30];
}Pau_Delta_Table_t;

void Init_Detection_Table();
void Init_Delta_Table();

PAU_Detect_Table_t* Get_Detec_Table();
Pau_Delta_Table_t* Get_Delta_Table();

void Save_Detection_Table(uint16_t addr);
void Save_FreqDelta_Table(uint16_t addr);
void Save_FreqDelta_Origin();

int16_t Get_DetectPower(uint16_t Detect);
int16_t Get_DeltaPower(uint32_t Frequency);

#endif /* TABLE_H_ */