/*
 * Uart_Device.h
 *
 * Created: 2022-07-17 오전 3:07:20
 *  Author: jjh83
 */ 


#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <sysconf.h>

typedef struct ringbuffer
{
	uint8_t* Buffer;
	int input_p;
	int output_p;
	int Length;
}ringbuffer_t;

void Init_UART(long baud, uint16_t max_recv_cnt);

uint8_t Uart_Txs(uint8_t* pkt,uint8_t size);
uint8_t Uart_Tx(uint8_t data);
uint8_t Uart_Rx(uint8_t* data);
void Uart_Flush();

uint16_t Uart_Rx_DTR();

#endif /* UART_DEVICE_H_ */