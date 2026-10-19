/*
 * HMC83x.h
 *
 * Created: 2022-07-17 오전 1:50:08
 *  Author: jjh83
 */ 


#ifndef HMC83X_H_
#define HMC83X_H_

#include <stdbool.h>

#include <util/delay.h>

#include <gpio.h>
#include <Spi_Device.h>

#define HMC_REG_REFDIV		0x02
#define HMC_REG_NDIV		0x03
#define HMC_REG_FRACDIV		0x04
#define HMC_REG_VCO			0x05
#define HMC_REG_SD_CFG		0x06
#define HMC_REG_LockDetect	0x07
#define HMC_REG_AnalogEN	0x08
#define HMC_REG_CP			0x09
#define HMC_REG_VCOCAL		0x0A
#define HMC_REG_PD			0x0B
#define HMC_REG_ExactFreq	0x0C
#define HMC_REG_GPO			0x0F

#define HMC_VCO_REG_EN		0x01
#define HMC_VCO_REG_Bias	0x02
#define HMC_VCO_REG_Config	0x03
#define HMC_VCO_REG_Cal		0x04
#define HMC_VCO_REG_CFCal	0x05
#define HMC_VCO_REG_MSGCal	0x06

#define HMC_FRAC_CONST_FULL	16777216
#define HMC_FRAC_CONST		16777
#define HMC_FRAC_CONST_DOUB	1678 * 5

enum
{
	HMC_VCO_DIV32 = 0,
	HMC_VCO_DIV16,
	HMC_VCO_DIV8,
	HMC_VCO_DIV4,
	HMC_VCO_DIV2,
	HMC_VCO_DIV0,
	HMC_VCO_DOUBLE
};

typedef struct HMC_PLL_Status
{
	uint8_t ison;
	uint8_t FreqRange;
	uint32_t Frequency;	
}HMC_PLL_t;

void HMC_RF_Enable(gpio_pin_t en, uint8_t rfon);
void HMC_Set_FreqRange(gpio_pin_t en, uint8_t Range);

void Init_HMC(gpio_pin_t en);
void Set_HMC_Frequency(gpio_pin_t en, uint32_t Freq);
void Write_HMC(gpio_pin_t en, uint8_t Register, uint32_t Value);
void Write_HMC_VCO(gpio_pin_t en, uint8_t Register, uint32_t Value);

#endif /* HMC83X_H_ */