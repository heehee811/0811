/*
 * system.h
 *
 * Created: 2022-07-18 오후 4:55:33
 *  Author: user
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <avr/eeprom.h>
#include <avr/wdt.h>

#include <stdint-gcc.h>
#include <stdbool.h>

#define System_Setting_Section __attribute__ ((section (".sys_setting")))

enum Alarm_State
{
	Normal = 0,
	Over_Power,     //1
	Over_Temp,      //2
	VSWR_Alarm,     //3
	ALC_Shutdown,   //4
	PLL_Fail,       //5
	Low_Power,      //6
	VDS_Fail        //7
};

enum RF_State
{
	RF_ON = 0,
	RF_OFF
};

enum PAU_State
{
	SYSTEM_IDLE = 0,
	SYSTEM_Tuning,
	SYSTEM_RFOUT,
	SYSTEM_Setting,
	SYSTEM_Alarm
};

typedef union PAU_Function_Enable
{
	uint8_t All;
	struct
	{
		uint8_t ALC_Enable:1;									//ALC Enable
		uint8_t ALC_Shutdown_Enable:1;        //ALC Shutdown
		uint8_t SWR_Shutdown_Enable:1;        //VSWR Shutdown
	};
}PAU_Function_Enable_t;

typedef struct PAU_System_Setting
{
	uint8_t Setted;
	uint8_t Information[15];
	PAU_Function_Enable_t Funcs;
	
	int8_t Display_Offset;
	int8_t Power_Offset;
	
	int8_t DSA_Preset;
	
	uint8_t Module_Address;
}Pau_Setting_t;

//System Flag
typedef union
{
	uint16_t All;
	struct
	{
		uint16_t Debug:1;						//System Debugging Mode
		uint16_t RFFlag:1;					//RF ON/OFF
		uint16_t RfLDFlag:1;        //RfPLL Lock Detection Check
		uint16_t LoLDFlag:1;        //LoPLL Lock Detection Check
		uint16_t FanFlag:1;					//FAN ON/OFF Status
		uint16_t dispflag:1;				//Display Refresh
		uint16_t Encflag:1;					//Encoder Flag
		uint16_t DetFlag:1;					//Detector Fix Flag
	};
}PAU_SystemFlag;

typedef struct Pau_Status
{
	int16_t Temp;
	uint16_t FWD_ADC;
	uint16_t RVS_ADC;
	int16_t FWD;
	int16_t RVS;
	int16_t RL;
	int16_t VVA_ATT;
	int16_t Delta;	
	
	uint32_t Frequency;
	uint16_t Power;
	
	uint8_t Alarm;
	uint8_t Status;
	
	PAU_SystemFlag SysFlag;
}Pau_Status_t;

void Load_System_Settings();
void Save_System_Settings();

void Set_DebugMode();
uint8_t Get_DebugMode();

Pau_Setting_t* Get_Settings();
Pau_Status_t* Get_Status();

void Set_System_Mode(uint8_t SMode);
uint8_t Get_System_Mode();

void System_Reset();
void Display_Changed();

#endif /* SYSTEM_H_ */