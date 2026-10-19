/*
 * Display.c
 *
 * Created: 2022-07-18 오후 5:43:16
 *  Author: user
 */ 
#include <Display.h>

uint8_t Disp_Line;

void Task_Display(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr)
{
	static uint8_t mode = SYSTEM_IDLE;
	if(stat_ptr->SysFlag.Debug == false)
	{
		return;
	}
// 	else if(stat_ptr->SysFlag.dispflag == false)
// 	{
// 		return;
// 	}
	
	if(mode != Get_System_Mode())
	{
		Clcd_Command(CMD_DISPLAY_CLR);
		mode = Get_System_Mode();
	}
	
	switch(mode)
	{
		case SYSTEM_IDLE:
		case SYSTEM_RFOUT:
		case SYSTEM_Tuning:
			Display_Main(stat_ptr,set_ptr);
			break;
		case SYSTEM_Alarm:
			Display_Alarm(stat_ptr,set_ptr);
			break;
			
		case SYSTEM_Setting:
			Display_Settings(stat_ptr,set_ptr);
			break;
	}
	
	//stat_ptr->SysFlag.dispflag = false;
	
	if(++Disp_Line >= 4)
	{
		Disp_Line = 0;
	}
}

void Display_Main(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr)
{
	switch(Disp_Line)
	{
		case 0:
		Clcd_Command(LINE1);
		printf
		(
		"F:%4lu.%02luMHz Po:%2d.%d",
		stat_ptr->Frequency / 100L,
		stat_ptr->Frequency % 100L,
		stat_ptr->Power / 10,
		stat_ptr->Power % 10
		);
		break;
		
		case 1:
		Clcd_Command(LINE2);
		printf
		(
		"F:%4d R:%4d P:%2d.%d",
		stat_ptr->FWD_ADC,
		stat_ptr->RVS_ADC,
		stat_ptr->FWD / 10,
		stat_ptr->FWD % 10
		);
		break;
		
		case 2:
		Clcd_Command(LINE3);
		printf
		(
		"D:%2d V:%4d R:%2dR%dL%d",
		set_ptr->DSA_Preset,
		stat_ptr->VVA_ATT,
		Uart_Rx_DTR(),
		stat_ptr->SysFlag.RfLDFlag,
		stat_ptr->SysFlag.LoLDFlag
		);
		break;
		
		case 3:
		
		Clcd_Command(LINE4);
		printf("T:%3d%cC %3d",stat_ptr->Temp/10, 0xDF, stat_ptr->Delta);
		break;
	}
}

void Display_Alarm(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr)
{
	if(Disp_Line == 0)
	{
		Clcd_Command(LINE1);
		switch(stat_ptr->Alarm)
		{
			case VSWR_Alarm:
				printf("PAU VSWR Alarm!");
				break;
			case ALC_Shutdown:
				printf(" ALC Shutdown! ");
				break;
			case PLL_Fail:
				printf("PAU PLL Unlock!");
				break;
			case Over_Temp:
				printf("PAU Over Temp!");
		}
	}
}

void Display_Settings(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr)
{
	if(Disp_Line == 0)
	{
		Clcd_Command(LINE1);
		printf("PAU Address : %02x",set_ptr->Module_Address);
	}
}