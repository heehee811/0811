/*
 * gpio.h
 *
 * Created: 2022-07-16 오후 7:28:42
 *  Author: jjh83
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>
/*
 * gpio.h
 *
 * Created: 2022-07-16 오후 7:28:42
 *  Author: jjh83
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>

#include <stdbool.h>

typedef struct gpio
{
	volatile uint8_t pin;
	volatile uint8_t ddr;
	volatile uint8_t port;
}gpio_t;

typedef struct gpio_pin
{
	volatile gpio_t* gpio_struct;
	volatile uint8_t pinnumber;	
}gpio_pin_t;

void Set_gpio_Direction_Input(gpio_t* gpio, uint8_t pinnumber);
void Set_gpio_Direction_Output(gpio_t* gpio, uint8_t pinnumber);
void Set_gpio_Output_Low(gpio_t* gpio, uint8_t pinnumber);
void Set_gpio_Output_High(gpio_t* gpio, uint8_t pinnumber);
void Set_gpio_Output_Toggle(gpio_t* gpio, uint8_t pinnumber);
uint8_t Get_gpio_Input(gpio_t* gpio,uint8_t pinnumber);

void Set_gpio_Pin_Direction_Input(gpio_pin_t gpio);
void Set_gpio_Pin_Direction_Output(gpio_pin_t gpio);
void Set_gpio_Pin_Output_Low(gpio_pin_t gpio);
void Set_gpio_Pin_Output_High(gpio_pin_t gpio);
void Set_gpio_Pin_Output_Toggle(gpio_pin_t gpio);
void Set_gpio_Pin_Output(gpio_pin_t gpio, uint8_t value);
uint8_t Get_gpio_Pin_Input(gpio_pin_t gpio);

#endif /* GPIO_H_ */
#include <stdbool.h>

typedef struct gpio
{
	volatile uint8_t pin;
	volatile uint8_t ddr;
	volatile uint8_t port;
}gpio_t;

typedef struct gpio_pin
{
	volatile gpio_t* gpio_struct;
	volatile uint8_t pinnumber;	
}gpio_pin_t;

void Set_gpio_Direction_Input(gpio_t* gpio, uint8_t pinnumber);
void Set_gpio_Direction_Output(gpio_t* gpio, uint8_t pinnumber);
void Set_gpio_Output_Low(gpio_t* gpio, uint8_t pinnumber);
void Set_gpio_Output_High(gpio_t* gpio, uint8_t pinnumber);
uint8_t Get_gpio_Input(gpio_t* gpio,uint8_t pinnumber);

void Set_gpio_Pin_Direction_Input(gpio_pin_t gpio);
void Set_gpio_Pin_Direction_Output(gpio_pin_t gpio);
void Set_gpio_Pin_Output_Low(gpio_pin_t gpio);
void Set_gpio_Pin_Output_High(gpio_pin_t gpio);
void Set_gpio_Pin_Output(gpio_pin_t gpio, uint8_t value);
uint8_t Get_gpio_Pin_Input(gpio_pin_t gpio);

#endif /* GPIO_H_ */