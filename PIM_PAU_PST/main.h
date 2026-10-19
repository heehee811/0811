/*
 * main.h
 *
 * Created: 2022-07-16 오후 7:38:43
 *  Author: jjh83
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <util/atomic.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sysconf.h>
#include <sysdef.h>
#include <MA.h>
#include <board.h>
#include <system.h>
#include <systick.h>
#include <Display.h>
#include <Table.h>

#include <gpio.h>
#include <Spi_Device.h>
#include <Uart_Device.h>
#include <ADC.h>

#include <CLCD.h>
#include <BVA.h>
#include <DAC7331.h>
#include <HMC83x.h>
#include <ADS7886.h>
#include <Encoder.h>


void Task_Packet();

void Task_Atten(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr);
void Task_Encoder(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr);
void Task_ALC(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr);
void Task_Detector(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr);
void Task_VSWR(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr);
void Task_RFPath(Pau_Status_t* stat_ptr);
void Task_PLL_Lockdetect(Pau_Status_t* stat_ptr);
void Task_Temp(Pau_Status_t* stat_ptr);
void Task_FAN(Pau_Status_t* stat_ptr);
void Task_Alarm(Pau_Status_t* stat_ptr);
void Task_Indicator(Pau_Status_t* stat_ptr);
void Task_Button();

void Init_System();
void Init_Port();
void Init_EINT();
void Init_TIM();
void Init_Display();

#endif /* MAIN_H_ */