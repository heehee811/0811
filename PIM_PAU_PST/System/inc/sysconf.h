/*
 * sysconf.h
 *
 * Created: 2022-07-16 오후 7:35:53
 *  Author: jjh83
 */ 


#ifndef SYSCONF_H_
#define SYSCONF_H_

#define	Version_Major								5
#define	Version_Minor								5
#define Version_Patch								2

#define Uart_Txd_BufferSize					100
#define Uart_Rxd_BufferSize					100

#define System_AREF									5000

#define System_Fan_High_Threshold		45 * 10
#define System_Fan_Low_Threshold		35 * 10
#define System_Temp_Limit						85 * 10
#define VSWR_Threshold							10 * 10

#define Start_Power									15 * 10
#define End_Power										48 * 10

#define Table_Length								End_Power - Start_Power + 1

#endif /* SYSCONF_H_ */