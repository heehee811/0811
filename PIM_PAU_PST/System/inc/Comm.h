/*
 * Comm.h
 *
 * Created: 2022-07-25 오전 9:55:40
 *  Author: user
 */ 


#ifndef COMM_H_
#define COMM_H_

#include <Packet.h>
#include <system.h>
#include <Uart_Device.h>
#include <Display.h>
#include <gpio.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <Table.h>

typedef struct comm_cmd
{
	char    cmd;
	uint8_t    (*comm_func)(Pkt_Buffer_t*,Pkt_Buffer_t*);
} comm_cmd_t;

void Init_Comm(gpio_pin_t rs485);

void Register_Handlers(uint8_t code, uint8_t (*func)(Pkt_Buffer_t*,Pkt_Buffer_t*));

void Comm_Handler();

uint8_t Get_Information(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Set_Information(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Get_Offset(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Set_Offset(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Reset_System(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Set_Frequency(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Get_Frequency(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Set_Power(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Get_power(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Set_Output_ON(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Set_Output_OFF(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Get_FWD_ATT(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Get_PAU_Status(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Set_TuningMode(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Exit_TuningMode(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Get_SystemAlarm(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Set_DSAtten(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Get_DSAtten(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Set_VVAtten(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Clr_FanAlarm(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Get_Version(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Get_Function_Enable(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Set_Function_Enable(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Save_Detect_Table(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Load_Detect_Table(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Save_Delta_Origin(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Load_Delta_Origin(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Save_Delta_Table(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);

uint8_t Load_Delta_Table(Pkt_Buffer_t* rx, Pkt_Buffer_t* tx);
#endif /* COMM_H_ */