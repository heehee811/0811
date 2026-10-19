/*
 * CLCD.c
 *
 * Created: 2022-07-16 오후 8:00:09
 *  Author: jjh83
 */ 
#include <CLCD.h>

FILE * Clcd_Stream;

volatile gpio_t * Clcd_ctrl;
volatile gpio_t * Clcd_data;
volatile uint8_t Clcd_rs;
volatile uint8_t Clcd_rw;
volatile uint8_t Clcd_en;

void Clcd_Open(gpio_t * ctrl, gpio_t * data, uint8_t rs, uint8_t rw, uint8_t en)
{
    Clcd_Stream = fdevopen(Clcd_Putchar,NULL);
    Clcd_ctrl = ctrl;
    Clcd_data = data;
    Clcd_rs = rs;
    Clcd_rw = rw;
    Clcd_en = en;
}

void Clcd_Init()
{
    Set_gpio_Direction_Output(Clcd_ctrl, Clcd_rs);
    Set_gpio_Direction_Output(Clcd_ctrl, Clcd_rw);
    Set_gpio_Direction_Output(Clcd_ctrl, Clcd_en);
    
    Clcd_data->ddr = 0xFF;
    Clcd_data->port = 0x00;

    Clcd_Command(CMD_8BIT_MODE);
	Clcd_Command(CMD_ENTRY_MODE_SET);
	Clcd_Command(CMD_DISPLAY_CLR);
	Clcd_Command(CMD_DISPLAY_CONTROL | D_Bit);
}

int Clcd_Putchar(uint8_t ch, FILE * fp)
{
    Clcd_Data(ch);
}

void Clcd_Command(uint8_t command)
{
    Clcd_mode_Instruction();
	Clcd_mode_Write();
	Clcd_Enable();
	_delay_us(100);
	Clcd_data->port = command;

	Clcd_Disable();
	_delay_ms(2);
}

void Clcd_Data(uint8_t data)
{
    Clcd_mode_Command();
	Clcd_mode_Write();
	Clcd_Enable();
	_delay_us(100);
	Clcd_data->port = data;

	Clcd_Disable();
	_delay_us(70);
}

void Clcd_mode_Instruction()
{
    Set_gpio_Output_Low(Clcd_ctrl, Clcd_rs);
}

void Clcd_mode_Command()
{
    Set_gpio_Output_High(Clcd_ctrl, Clcd_rs);
}

void Clcd_mode_Write()
{
    Set_gpio_Output_Low(Clcd_ctrl, Clcd_rw);
}

void Clcd_mode_Read()
{
    Set_gpio_Output_High(Clcd_ctrl, Clcd_rw);
}

void Clcd_Enable()
{
    Set_gpio_Output_High(Clcd_ctrl, Clcd_en);
}

void Clcd_Disable()
{
    Set_gpio_Output_Low(Clcd_ctrl, Clcd_en);
}