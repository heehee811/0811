/*
 * Comm.c
 *
 * Created: 2022-07-25 오전 9:55:48
 *  Author: user
 */ 

#include <Comm.h>

volatile comm_cmd_t Handlers[PIMA_Comm_Handler_Size];
volatile uint8_t handler_cnt = 0;

Pau_Setting_t* comm_setting_ptr;
Pau_Status_t* comm_status_ptr;

PAU_Detect_Table_t* comm_detect_ptr;
Pau_Delta_Table_t* comm_delta_ptr;

gpio_pin_t comm_485_en;

uint16_t ASCII_To_uint16(uint8_t* Str)
{
	uint16_t a = 0;

	a+= (uint16_t)(((*Str++)-0x30)*10000);
	a+= (uint16_t)(((*Str++)-0x30)*1000);
	a+= (uint16_t)(((*Str++)-0x30)*100);
	a+= (uint16_t)(((*Str++)-0x30)*10);
	a+= (uint16_t)((*Str++)-0x30);

	return a;
}

void Init_Comm(gpio_pin_t rs485)
{
	comm_setting_ptr = Get_Settings();
	comm_status_ptr = Get_Status();
	
	comm_detect_ptr = Get_Detec_Table();
	comm_delta_ptr = Get_Delta_Table();
	
	comm_485_en = rs485;

	Register_Handlers(0x1A,Get_PAU_Status);
	
	Register_Handlers(0x52, Set_Function_Enable);
	Register_Handlers(0x54, Get_Function_Enable);
	
	Register_Handlers(0x56,Set_Offset);
	Register_Handlers(0x58,Get_Offset);
	
	Register_Handlers(0x10, Save_Delta_Origin);
	Register_Handlers(0x12, Load_Delta_Origin);
	
	Register_Handlers(0x14, Save_Delta_Table);
	Register_Handlers(0x16, Load_Delta_Table);
	
	Register_Handlers(0x00,Get_Version);
	
	Register_Handlers(0x72,Get_FWD_ATT);
	
	Register_Handlers(0x70,Set_Output_ON);
	Register_Handlers(0x80,Set_Output_OFF);
	Register_Handlers(0x90,Get_SystemAlarm);
	
	Register_Handlers(0xAA,System_Reset);
	
	Register_Handlers(0xD0,Set_Information);
	Register_Handlers(0x02,Get_Information);
	Register_Handlers(0xDA,Get_Information);
	
	//Register_Handlers(0x50,Set_Frequency);
	Register_Handlers(0x5A,Set_Frequency);
	Register_Handlers(0xE0,Get_Frequency);
	
	//Register_Handlers(0x60,Set_Power);
	Register_Handlers(0x6A,Set_Power);
	Register_Handlers(0xF0,Get_power);
	
	Register_Handlers(0xBA,Set_TuningMode);
	Register_Handlers(0xBC,Exit_TuningMode);
	Register_Handlers(0xBE,Set_VVAtten);
	
	Register_Handlers(0xAE,Set_DSAtten);
	Register_Handlers(0xAC,Get_DSAtten);
	
	Register_Handlers(0x60,Load_Detect_Table);
	Register_Handlers(0x62,Save_Detect_Table);
}

void Register_Handlers(uint8_t code, uint8_t (*func)(Pkt_Buffer_t*,Pkt_Buffer_t*))
{
	Handlers[handler_cnt].cmd = code;
	Handlers[handler_cnt++].comm_func = func;
}

void Comm_Handler()
{
	Pkt_Buffer_t* rx_buffer;
	Pkt_Buffer_t* tx_buffer;
	
	uint8_t processed = false;
	uint8_t i = 0;
	
	rx_buffer = Get_Packet_Receive_Buffer();
	tx_buffer = Get_Packet_Transmit_Buffer();
	
	tx_buffer->cnt = 0;
	
	for(;i<handler_cnt;i++)
	{
		if(Handlers[i].cmd == rx_buffer->Buffer[Rx_Command])
		{
			Set_gpio_Pin_Output_High(comm_485_en);
			//Tx Driver Enable Delay Max 5500us
			_delay_ms(6);
			Handlers[i].comm_func(rx_buffer, tx_buffer);
			processed = true;
			Uart_Txs(Header_Packet,3);
			Uart_Tx(Get_Module_Address());
			Uart_Tx(Handlers[i].cmd + 1);
			Uart_Tx(tx_buffer->cnt);
			Uart_Txs(tx_buffer->Buffer,15);
			Uart_Txs(Tailor_Packet,3);
			
			//Rx Driver Enable Delay Max 2200us
			_delay_ms(3);
			break;
		}
	}
	memset(tx_buffer->Buffer,0,15);
	Init_PacketReceive();
	Set_gpio_Pin_Output_Low(comm_485_en);
}

uint8_t Get_Information(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	memcpy(tx->Buffer,comm_setting_ptr->Information,15);
	
	tx->cnt = 15;
	return true;
}

uint8_t Set_Information(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	memcpy(comm_setting_ptr->Information,rx->Buffer + Rx_Data,15);
	
	Save_System_Settings();

	return true;
}

uint8_t Get_Offset(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	/*tx->cnt = sprintf(
		tx->Buffer,"%03d%03d",
		comm_setting_ptr->Display_Offset,
		comm_setting_ptr->Power_Offset);*/
	
	tx->Buffer[0] = ((comm_setting_ptr->Display_Offset >> 4) & 0x0F) + 0x30;
	tx->Buffer[1] = ((comm_setting_ptr->Display_Offset >> 0) & 0x0F) + 0x30;
	
	tx->Buffer[2] = ((comm_setting_ptr->Power_Offset >> 4) & 0x0F) + 0x30;
	tx->Buffer[3] = ((comm_setting_ptr->Power_Offset >> 0) & 0x0F) + 0x30;
	
	tx->cnt = 4;
	
	return true;
}

uint8_t Set_Offset(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	/*comm_setting_ptr->Display_Offset = atoi(rx->Buffer+Rx_Data);
	
	comm_setting_ptr->Power_Offset = atoi(rx->Buffer+Rx_Data+4);*/
	
	comm_setting_ptr->Display_Offset = rx->Buffer[Rx_Data] & 0x0F;
	comm_setting_ptr->Display_Offset <<= 4;
	comm_setting_ptr->Display_Offset += rx->Buffer[Rx_Data+1] & 0x0F;
	
	comm_setting_ptr->Power_Offset = rx->Buffer[Rx_Data+2] & 0x0F;
	comm_setting_ptr->Power_Offset <<= 4;
	comm_setting_ptr->Power_Offset += rx->Buffer[Rx_Data+3] & 0x0F;
	
	Save_System_Settings();
	
	return true;
}

uint8_t Reset_System(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	System_Reset();
}

uint8_t Set_Frequency(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	comm_status_ptr->Frequency = atol(rx->Buffer+Rx_Data);
	
	return true;
}

uint8_t Get_Frequency(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	tx->cnt = sprintf(tx->Buffer,"%06ld",comm_status_ptr->Frequency);
	return true;
}

uint8_t Set_Power(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	comm_status_ptr->Power = atoi(rx->Buffer+Rx_Data);
	
	return true;
}

uint8_t Get_power(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	tx->cnt = sprintf(tx->Buffer,"%03d",comm_status_ptr->Power);
	return true;
}

uint8_t Set_Output_ON(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	Set_System_Mode(SYSTEM_RFOUT);
	return true;
}

uint8_t Set_Output_OFF(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	Set_System_Mode(SYSTEM_IDLE);
	return true;
}

uint8_t Get_FWD_ATT(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	Pau_Status_t* stat = Get_Status();
	tx->cnt = sprintf(tx->Buffer,"%04d%04d",
		comm_status_ptr->FWD_ADC,
		comm_status_ptr->VVA_ATT);
	return true;
}

uint8_t Get_PAU_Status(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	tx->cnt = sprintf(tx->Buffer,"%03d%03d%03d%c",
		comm_status_ptr->FWD,
		comm_status_ptr->RL,
		comm_status_ptr->Temp,
		comm_status_ptr->Alarm);
	
	return true;
}

uint8_t Set_TuningMode(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	Set_System_Mode(SYSTEM_Tuning);
	return true;
}

uint8_t Exit_TuningMode(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	Set_System_Mode(SYSTEM_IDLE);
	return true;
}

uint8_t Get_SystemAlarm(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	tx->Buffer[tx->cnt] = comm_status_ptr->Alarm;
	
	return true;
}

uint8_t Set_DSAtten(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	comm_setting_ptr->DSA_Preset = atoi(rx->Buffer+Rx_Data);
	
	Save_System_Settings();
	
	return true;
}

uint8_t Get_DSAtten(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	tx->cnt = sprintf(
		tx->Buffer,"%04d",
		comm_setting_ptr->DSA_Preset);
	
	return true;
}

uint8_t Set_VVAtten(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	comm_status_ptr->VVA_ATT = atoi(rx->Buffer+Rx_Data);
	
	return true;
}

uint8_t Get_Version(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	tx->cnt = sprintf(tx->Buffer,"%d.%d.%d%c",
		Version_Major,
		Version_Minor,
		Version_Patch,
		0x00);
	
	return true;
}

uint8_t Get_Function_Enable(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	tx->Buffer[0] = comm_setting_ptr->Funcs.All;
	
	tx->cnt = 1;
	return true;
}

uint8_t Set_Function_Enable(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	if(rx->Buffer[Rx_Data] == 0xE6)
	{
		comm_setting_ptr->Funcs.ALC_Enable = true;
	}
	else
	{
		comm_setting_ptr->Funcs.ALC_Enable = false;
	}
	
	if(rx->Buffer[Rx_Data+1] == 0xE7)
	{
		comm_setting_ptr->Funcs.ALC_Shutdown_Enable = true;
	}
	else
	{
		comm_setting_ptr->Funcs.ALC_Shutdown_Enable = false;
	}
	
	if(rx->Buffer[Rx_Data+2] == 0xE8)
	{
		comm_setting_ptr->Funcs.SWR_Shutdown_Enable = true;
	}
	else
	{
		comm_setting_ptr->Funcs.SWR_Shutdown_Enable = false;
	}
	
	Save_System_Settings();
	
	return true;
}

uint8_t Save_Detect_Table(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	uint8_t i = Rx_Data;
	uint16_t cnt = 0;
	uint16_t att = 0;
	uint16_t det = 0;
	
	cnt += (rx->Buffer[i++] & 0x0F) * 100;
	cnt += (rx->Buffer[i++] & 0x0F) * 10;
	cnt += (rx->Buffer[i++] & 0x0F) * 1;
	
	att += (rx->Buffer[i++] & 0x0F) * 1000;
	att += (rx->Buffer[i++] & 0x0F) * 100;
	att += (rx->Buffer[i++] & 0x0F) * 10;
	att += (rx->Buffer[i++] & 0x0F) * 1;
	
	det += (rx->Buffer[i++] & 0x0F) * 1000;
	det += (rx->Buffer[i++] & 0x0F) * 100;
	det += (rx->Buffer[i++] & 0x0F) * 10;
	det += (rx->Buffer[i++] & 0x0F) * 1;
	
	comm_detect_ptr->Table[cnt].Atten = att;
	comm_detect_ptr->Table[cnt].Detect = det;
	comm_detect_ptr->Setted = 0x00;
	
	Save_Detection_Table(cnt);
	
	return true;
}

uint8_t Load_Detect_Table(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	uint8_t i = Rx_Data;
	uint16_t cnt = 0;

	cnt += (rx->Buffer[i++] & 0x0F) * 100;
	cnt += (rx->Buffer[i++] & 0x0F) * 10;
	cnt += (rx->Buffer[i++] & 0x0F) * 1;

	tx->cnt = sprintf(tx->Buffer,"%04d%04d",
	comm_detect_ptr->Table[cnt].Atten,
	comm_detect_ptr->Table[cnt].Detect);
	return true;
}

uint8_t Save_Delta_Origin(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	uint8_t		i = Rx_Data;
	uint16_t	cnt = 0;
	uint16_t	Freq = 0;
	int8_t		Delta = 0;

	Freq += (rx->Buffer[i++] & 0x0F) * 10000;
	Freq += (rx->Buffer[i++] & 0x0F) * 1000;
	Freq += (rx->Buffer[i++] & 0x0F) * 100;
	Freq += (rx->Buffer[i++] & 0x0F) * 10;
	Freq += (rx->Buffer[i++] & 0x0F) * 1;
	
	Delta = (rx->Buffer[i++] & 0x0F);
	Delta <<= 4;
	Delta |= (rx->Buffer[i++] & 0x0F);
	
	comm_delta_ptr->Origin.Delta = Delta;
	comm_delta_ptr->Origin.Freq = Freq;
	
	tx->cnt = sprintf(tx->Buffer,"%05u%02x",comm_delta_ptr->Origin.Freq, comm_delta_ptr->Origin.Delta);
	
	Save_FreqDelta_Origin();

	return true;
}

uint8_t Load_Delta_Origin(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	sprintf(tx->Buffer,"%05u",
	comm_delta_ptr->Origin.Freq);
	
	tx->Buffer[5] = (comm_delta_ptr->Origin.Delta >> 4) + 0x30;
	tx->Buffer[6] = (comm_delta_ptr->Origin.Delta & 0x0F) + 0x30;
	tx->cnt = 7;
	//tx->cnt = sprintf(tx->Buffer,"%05u%02x",comm_delta_ptr->Origin.Freq, comm_delta_ptr->Origin.Delta);
	return true;
}

uint8_t Save_Delta_Table(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	uint8_t i = Rx_Data;
	uint8_t ptr = 0;
	uint16_t Freq = 0;
	int8_t Delta = 0;
	
	ptr = rx->Buffer[i++];
	
	Freq += (rx->Buffer[i++] & 0x0F) * 10000;
	Freq += (rx->Buffer[i++] & 0x0F) * 1000;
	Freq += (rx->Buffer[i++] & 0x0F) * 100;
	Freq += (rx->Buffer[i++] & 0x0F) * 10;
	Freq += (rx->Buffer[i++] & 0x0F);
	
	Delta = (rx->Buffer[i++] & 0x0F);
	Delta <<= 4;
	Delta |= (rx->Buffer[i++] & 0x0F);
	
	comm_delta_ptr->Tables[ptr].Freq = Freq;
	comm_delta_ptr->Tables[ptr].Delta = Delta;
	
	tx->cnt = sprintf(tx->Buffer,"%d%05u%02x",ptr, Freq, Delta);
	
	Save_FreqDelta_Table(ptr);
	return true;
}

uint8_t Load_Delta_Table(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx)
{
	sprintf(tx->Buffer,"%05u",
		comm_delta_ptr->Tables[rx->Buffer[Rx_Data]].Freq);
		
	tx->Buffer[5] = (comm_delta_ptr->Tables[rx->Buffer[Rx_Data]].Delta >> 4) + 0x30;
	tx->Buffer[6] = (comm_delta_ptr->Tables[rx->Buffer[Rx_Data]].Delta & 0x0F) + 0x30;
	tx->cnt = 7;
	return true;
}