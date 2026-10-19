/*
 * Packet.c
 *
 * Created: 2022-07-20 오전 11:01:13
 *  Author: user
 */ 
#include <Packet.h>
volatile rx_state_t pkt_rx_state = {0};
volatile Pkt_Buffer_t Pkt_Rx;
volatile Pkt_Buffer_t Pkt_Tx;
volatile uint32_t Pkt_Tick = 0;

void Init_PacketReceive()
{
	Pkt_Rx.cnt = 0;
	pkt_rx_state.All = 0;
	//Uart_Flush();
}

uint8_t Get_PacketStatus()
{
	return pkt_rx_state.avail;
}

uint8_t Analyze_Packet(uint8_t data)
{
	//timeout Checking
	if(pkt_rx_state.header != false)
	{
		if((Get_Systick() - Pkt_Tick) > 100)
		{
			Init_PacketReceive();
		}
	}
	

	if(pkt_rx_state.header == false)
	{
		if(data == PIMA_Packet_Flag)
		{
			pkt_rx_state.header = true;
			Pkt_Tick = Get_Systick();
		}
	}
				
	else
	{
		if(Pkt_Rx.cnt < PIMA_Packet_Size)
		{
			if(data == PIMA_Packet_Flag)
			{
				pkt_rx_state.avail = true;
			}
			else
			{
				Pkt_Rx.Buffer[Pkt_Rx.cnt++] = data;
			}
		}
		else
		{
			Init_PacketReceive();
		}
	}
	
	return pkt_rx_state.avail;
}

uint8_t Compare_Address()
{
	if(Pkt_Rx.Buffer[Rx_Address] == PIMA_ADDR_Broadcast)
	{
		return true;
	}
	else if(Pkt_Rx.Buffer[Rx_Address] == Get_Module_Address())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Set_Module_Address(uint8_t Address)
{
	Pau_Setting_t* sett = Get_Settings();
	
	sett->Module_Address = Address;
}

uint8_t Get_Module_Address()
{
	Pau_Setting_t* sett = Get_Settings();
	
	return sett->Module_Address;
}

Pkt_Buffer_t* Get_Packet_Receive_Buffer()
{
	return &Pkt_Rx;
}

Pkt_Buffer_t* Get_Packet_Transmit_Buffer()
{
	return &Pkt_Tx;
}