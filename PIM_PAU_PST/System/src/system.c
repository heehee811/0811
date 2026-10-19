/*
 * system.c
 *
 * Created: 2022-07-18 오후 5:18:32
 *  Author: user
 */ 
#include <system.h>

System_Setting_Section Pau_Setting_t Pau_SysSettings_eeprom;

Pau_Setting_t Pau_SysSetting;
Pau_Status_t Pau_SysStatus;

void Load_System_Settings()
{
	eeprom_read_block(&Pau_SysSetting, &Pau_SysSettings_eeprom, sizeof(Pau_Setting_t));
	if(Pau_SysSetting.Setted == 0xFF)
	{
		memcpy(Pau_SysSetting.Information," Innertron PAU ",15);
		//memset(Pau_SysSetting.Information, 0x00, 15);
		Pau_SysSetting.Funcs.ALC_Enable = false;
		Pau_SysSetting.Funcs.ALC_Shutdown_Enable = false;
		Pau_SysSetting.Funcs.SWR_Shutdown_Enable = false;
		
		Pau_SysSetting.Display_Offset = 0;
		Pau_SysSetting.Power_Offset = 0;
		
		Pau_SysSetting.DSA_Preset = 30;
		Pau_SysSetting.Module_Address = 0x00;
		
		Pau_SysSetting.Setted = 0xAA;
		Save_System_Settings();
	}
}

void Save_System_Settings()
{
	eeprom_update_block(&Pau_SysSetting, &Pau_SysSettings_eeprom, sizeof(Pau_Setting_t));
}

void Set_DebugMode()
{
	Pau_SysStatus.SysFlag.Debug = true;
}

uint8_t Get_DebugMode()
{
	return Pau_SysStatus.SysFlag.Debug;
}

Pau_Setting_t* Get_Settings()
{
	return &Pau_SysSetting;
}

Pau_Status_t* Get_Status()
{
	return &Pau_SysStatus;
}

void Set_System_Mode(uint8_t SMode)
{
	Pau_SysStatus.Status = SMode;
	Pau_SysStatus.SysFlag.DetFlag = true;
	switch(SMode)
	{
		case SYSTEM_IDLE:
			Pau_SysStatus.SysFlag.RFFlag = false;
			Pau_SysStatus.Alarm = Normal;
			break;
		case SYSTEM_Tuning:
			Pau_SysStatus.SysFlag.RFFlag = true;
			break;
		case SYSTEM_RFOUT:
			Pau_SysStatus.SysFlag.RFFlag = true;
			Pau_SysStatus.SysFlag.DetFlag = false;
			break;
	}
}

uint8_t Get_System_Mode()
{
	return Pau_SysStatus.Status;
}

void System_Reset()
{
	wdt_enable(WDTO_15MS);
	while(1);
}

void Display_Changed()
{
	Pau_SysStatus.SysFlag.dispflag = true;
}