/*
 * gpio.c
 *
 * Created: 2022-07-16 오후 7:56:34
 *  Author: jjh83
 */ 

#include <gpio.h>

void Set_gpio_Direction_Input(gpio_t* gpio, uint8_t pinnumber)
{
    gpio->ddr &= ~_BV(pinnumber);
}

void Set_gpio_Direction_Output(gpio_t* gpio, uint8_t pinnumber)
{
    gpio->ddr |= _BV(pinnumber);
}

void Set_gpio_Output_Low(gpio_t* gpio, uint8_t pinnumber)
{
    gpio->port &= ~_BV(pinnumber);
}

void Set_gpio_Output_High(gpio_t* gpio, uint8_t pinnumber)
{
    gpio->port |= _BV(pinnumber);
}

void Set_gpio_Output_Toggle(gpio_t* gpio, uint8_t pinnumber)
{
	gpio->port ^= _BV(pinnumber);
}

uint8_t Get_gpio_Input(gpio_t* gpio,uint8_t pinnumber)
{
	return ((gpio->pin & _BV(pinnumber)) > 0);
}

void Set_gpio_Pin_Direction_Input(gpio_pin_t gpio)
{
    Set_gpio_Direction_Input(gpio.gpio_struct, gpio.pinnumber);
}

void Set_gpio_Pin_Direction_Output(gpio_pin_t gpio)
{
    Set_gpio_Direction_Output(gpio.gpio_struct, gpio.pinnumber);
}

void Set_gpio_Pin_Output_Low(gpio_pin_t gpio)
{
    Set_gpio_Output_Low(gpio.gpio_struct, gpio.pinnumber);
}

void Set_gpio_Pin_Output_High(gpio_pin_t gpio)
{
    Set_gpio_Output_High(gpio.gpio_struct, gpio.pinnumber);
}

void Set_gpio_Pin_Output_Toggle(gpio_pin_t gpio)
{
	Set_gpio_Output_Toggle(gpio.gpio_struct, gpio.pinnumber);
}

void Set_gpio_Pin_Output(gpio_pin_t gpio, uint8_t value)
{
	if(value == false)
	{
		Set_gpio_Pin_Output_Low(gpio);
	}
	
	else
	{
		Set_gpio_Pin_Output_High(gpio);
	}
}

uint8_t Get_gpio_Pin_Input(gpio_pin_t gpio)
{
    return Get_gpio_Input(gpio.gpio_struct, gpio.pinnumber);
}