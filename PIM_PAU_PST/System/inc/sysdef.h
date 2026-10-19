/*
 * sysdef.h
 *
 * Created: 2022-07-16 오후 7:36:03
 *  Author: jjh83
 */ 


#ifndef SYSDEF_H_
#define SYSDEF_H_

#include <avr/io.h>
#include <gpio.h>

volatile gpio_t const* gpioa = (gpio_t*)&PINA;
volatile gpio_t const* gpiob = (gpio_t*)&PINB;
volatile gpio_t const* gpioc = (gpio_t*)&PINC;
volatile gpio_t const* gpiod = (gpio_t*)&PIND;
volatile gpio_t const* gpioe = (gpio_t*)&PINE;
volatile gpio_t const* gpiog = (gpio_t*)&PING;

static gpio_pin_t gpio_485_en;

static gpio_pin_t gpio_bva_en;
static gpio_pin_t gpio_vva_en;
static gpio_pin_t gpio_rfpll_en;
static gpio_pin_t gpio_lopll_en;
static gpio_pin_t gpio_adc_en;

static gpio_pin_t gpio_adc_rvsfwd;

static gpio_pin_t gpio_led_green;
static gpio_pin_t gpio_led_red;

static gpio_pin_t gpio_hpa_drv;
static gpio_pin_t gpio_hpa_vgs;
static gpio_pin_t gpio_hpa_vds;

static gpio_pin_t gpio_rfpll_ld;
static gpio_pin_t gpio_lopll_ld;

static gpio_pin_t gpio_fan;

static gpio_pin_t gpio_fan_alarm1;
static gpio_pin_t gpio_fan_alarm2;
static gpio_pin_t gpio_fan_alarm_clear;

static gpio_pin_t gpio_btn0;
static gpio_pin_t gpio_btn1;
#endif /* SYSDEF_H_ */