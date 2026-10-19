/*
 * MA.c
 *
 * Created: 3/22/2022 10:23:20 AM
 *  Author: user
 */ 
#include "MA.h"

int16_t Push_MA(MA_t *Struct, int16_t value)
{
	if(Struct->cnt1 < MA_SIZE)
		Struct->cnt1++;
		
	Struct->Data[Struct->cnt2++] = value;

	if(Struct->cnt2 >= MA_SIZE)
		Struct->cnt2 = 0;
	
	int32_t ACC = 0;

	for(uint8_t i=0;i<Struct->cnt1;i++)
	{
		ACC += Struct->Data[i];
	}
	return ACC/Struct->cnt1;
	
}

void Clear_MA(MA_t *Struct)
{
	Struct->cnt1 = 0;
	Struct->cnt2 = 0;
}