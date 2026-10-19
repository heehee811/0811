/*
 * MA.h
 *
 * Created: 3/22/2022 10:23:11 AM
 *  Author: user
 */ 


#ifndef MA_H_
#define MA_H_

#include <stdint-gcc.h>

#define MA_SIZE		10

typedef struct MA_Struct 
{
	int16_t Data[MA_SIZE];
	uint8_t cnt1;
	uint8_t cnt2;
}MA_t;

int16_t Push_MA(MA_t *Struct, int16_t value);

void Clear_MA(MA_t *Struct);

#endif /* MA_H_ */