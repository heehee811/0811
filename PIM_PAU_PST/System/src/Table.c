/*
 * Table.c
 *
 * Created: 2022-07-26 오전 10:23:44
 *  Author: user
 */ 
#include <Table.h>

volatile Detect_Table_Section PAU_Detect_Table_t	Detection_Table_eeprom;
volatile Delta_Table_Section	Pau_Delta_Table_t		Delta_Table_eeprom;

volatile PAU_Detect_Table_t		Detection_Table;
volatile Pau_Delta_Table_t		Delta_Table;

void Init_Detection_Table()
{
	eeprom_read_block(&Detection_Table, &Detection_Table_eeprom, sizeof(PAU_Detect_Table_t));
}

void Init_Delta_Table()
{
	eeprom_read_block(&Delta_Table, &Delta_Table_eeprom, sizeof(Pau_Delta_Table_t));
	if(Delta_Table.Origin.Delta < 0)
	{
		Delta_Table.Origin.Delta = 0;
	}
}

PAU_Detect_Table_t* Get_Detec_Table()
{
	return &Detection_Table;
}

Pau_Delta_Table_t* Get_Delta_Table()
{
	return &Delta_Table;
}

void Save_Detection_Table(uint16_t addr)
{
	eeprom_update_block(Detection_Table.Table + addr,Detection_Table_eeprom.Table + addr, sizeof(Pau_Detect_t));
	eeprom_update_byte(&Detection_Table_eeprom.Setted,Detection_Table.Setted);
}

void Save_FreqDelta_Table(uint16_t addr)
{
	eeprom_update_block(Delta_Table.Tables + addr,Delta_Table_eeprom.Tables + addr, sizeof(Pau_Delta_t));
}

void Save_FreqDelta_Origin()
{
	eeprom_update_block(&Delta_Table.Origin, &Delta_Table_eeprom.Origin, sizeof(Pau_Delta_t));
}

int16_t Get_DetectPower(uint16_t Detect)
{
	static uint16_t Current_index=0;
	int16_t cnt;
	uint8_t c1,c2;
	
	if(Detection_Table.Setted == 0xFF)
	{
		return 0;
	}
	
	
	if(Detect <= Detection_Table.Table[0].Detect)
	{
		return Start_Power;
	}
	else if(Detect >= Detection_Table.Table[Table_Length - 1].Detect)
	{
		return End_Power;
	}

	while(1)
	{
		c1 = (Detect >= Detection_Table.Table[Current_index].Detect);
		c2 = (Detect < Detection_Table.Table[Current_index+1].Detect);

		if(c1 && c2)
		{
			return Start_Power + Current_index;
		}
		else if(c1)
		{
			if(++Current_index > Table_Length-1)
			{
				return End_Power;
			}
		}
		else
		{
			if(--Current_index < 0)
			{
				return Start_Power;
			}
		}
	}
}

int16_t Get_DeltaPower(uint32_t Frequency)
{
	uint16_t Freq = (uint16_t)(Frequency / 10);
	uint8_t i=0;
	int16_t ret = 0;
	if(Delta_Table.Origin.Delta == 0)
	{
		return 0;
	}
	
	if( Freq < Delta_Table.Origin.Freq)
	{
		return 0;
	}
	
	for(;i<=Delta_Table.Origin.Delta;i++)
	{
		if(Freq < Delta_Table.Tables[i].Freq)
		{
			break;
		}
		
		ret += Delta_Table.Tables[i].Delta;
	}
	
	int16_t Freq_l , Freq_h;
	int16_t delta_l, delta_h;
	
	if(i != 0)
	{
		Freq_l = Delta_Table.Tables[i-1].Freq;
		Freq_h = Delta_Table.Tables[i].Freq;
		
		delta_l = (int16_t)Delta_Table.Tables[i-1].Delta;
		delta_h = (int16_t)Delta_Table.Tables[i].Delta;
	}
	
	else
	{
		Freq_l = Delta_Table.Origin.Freq;
		Freq_h = Delta_Table.Tables[i].Freq;
		
		delta_l = 0;
		delta_h = (int16_t)Delta_Table.Tables[i].Delta;
	}
	
	
	
	if(i != 0)
	{
		int16_t slope =	(delta_h - delta_l) * (Freq - Freq_l);
		slope /= (Freq_h - Freq_l);		

		ret += slope;
	}
	return ret;
}