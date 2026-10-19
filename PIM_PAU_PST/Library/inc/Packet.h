/*
 * Packet.h
 *
 * Created: 2022-07-20 오전 10:57:23
 *  Author: user
 */ 


#ifndef PACKET_H_
#define PACKET_H_

#include <stdint-gcc.h>
#include <stdbool.h>

#include <systick.h>

#include <system.h>

#define PIMA_ADDR_Broadcast		0xFF
#define PIMA_Packet_Flag		0x7E
#define PIMA_Packet_Size		24
#define PIMA_Comm_Handler_Size	30

#define Rx_Address  0
#define Rx_Command  1
#define Rx_Length   2
#define Rx_Data			3

#define Header_Packet		"#@~"

static const uint8_t Tailor_Packet[3] = {0x00,0x00,0x7E};

typedef struct Pkt_Buffer
{
	uint8_t Buffer[30];
	uint8_t cnt;
}Pkt_Buffer_t;

typedef union rx_state
{
	uint8_t All;
	struct
	{
		uint8_t header:1;
		uint8_t avail:1;
		uint8_t padding:6;
	};
}rx_state_t;

void Init_PacketReceive();
uint8_t Get_PacketStatus();
uint8_t Analyze_Packet(uint8_t data);
uint8_t Compare_Address();

void Set_Module_Address(uint8_t Address);
uint8_t Get_Module_Address();

Pkt_Buffer_t* Get_Packet_Receive_Buffer();
Pkt_Buffer_t* Get_Packet_Transmit_Buffer();

#endif /* PACKET_H_ */