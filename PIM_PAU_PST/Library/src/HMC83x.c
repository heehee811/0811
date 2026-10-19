/*
 * HMC83x.c
 *
 * Created: 2022-07-18 오전 10:21:58
 *  Author: user
 */ 
#include <HMC83x.h>

void HMC_RF_Enable(gpio_pin_t en, uint8_t rfon)
{
	uint8_t value = 0x00;
	if(rfon != false)
	{
		value = 0x1F;
	}
	Write_HMC_VCO(en, HMC_VCO_REG_EN, value);
}

void HMC_Set_FreqRange(gpio_pin_t en, uint8_t Range)
{
	if(Range == HMC_VCO_DOUBLE)
	{
		Write_HMC_VCO(en, HMC_VCO_REG_Bias, 0x0C1);
		Write_HMC_VCO(en, HMC_VCO_REG_Config, 0x040);
	}
	else if(Range == HMC_VCO_DIV0)
	{
		Write_HMC_VCO(en, HMC_VCO_REG_Bias, 0x101);
		Write_HMC_VCO(en, HMC_VCO_REG_Config, 0x051);
	}
	else if(Range == HMC_VCO_DIV2)
	{
		Write_HMC_VCO(en, HMC_VCO_REG_Bias, 0x102);
		Write_HMC_VCO(en, HMC_VCO_REG_Config, 0x051);
	}
	else if(Range == HMC_VCO_DIV4)
	{
		Write_HMC_VCO(en, HMC_VCO_REG_Bias, 0x004);
		Write_HMC_VCO(en, HMC_VCO_REG_Config, 0x051);
	}
	else if(Range == HMC_VCO_DIV8)
	{
		Write_HMC_VCO(en, HMC_VCO_REG_Bias, 0x0C8);
		Write_HMC_VCO(en, HMC_VCO_REG_Config, 0x051);
	}
	else if(Range == HMC_VCO_DIV16)
	{
		Write_HMC_VCO(en, HMC_VCO_REG_Bias, 0x0D0);
		Write_HMC_VCO(en, HMC_VCO_REG_Config, 0x051);
	}
	else if(Range == HMC_VCO_DIV32)
	{
		Write_HMC_VCO(en, HMC_VCO_REG_Bias, 0x0E0);
		Write_HMC_VCO(en, HMC_VCO_REG_Config, 0x051);
	}
	
	Write_HMC_VCO(en, 0x00, 0x00000);
}

void Init_HMC(gpio_pin_t en)
{
	Write_HMC(en, HMC_REG_GPO, 0x000081);
	Write_HMC(en, HMC_REG_REFDIV, 0x01);
	Write_HMC(en, HMC_REG_VCOCAL, 0x002046);
	Write_HMC(en, HMC_REG_PD, 0x07E061);
	Write_HMC(en, HMC_REG_CP, 0x402FBF);
}

void Set_HMC_Frequency(gpio_pin_t en, uint32_t Freq)
{
	uint32_t N;
	
	uint64_t Frac;
	
	Init_HMC(en);	
	
	if(Freq < 9375)
	{
		Freq <<= 5;

		HMC_Set_FreqRange(en,HMC_VCO_DIV32);
	}
	else if(Freq < 18750)
	{
		Freq <<= 4;

		HMC_Set_FreqRange(en,HMC_VCO_DIV16);
	}
	else if(Freq < 37500)
	{
		Freq <<= 3;

		HMC_Set_FreqRange(en,HMC_VCO_DIV8);
	}
	else if(Freq < 75000)
	{
		Freq <<= 2;

		HMC_Set_FreqRange(en,HMC_VCO_DIV4);	
	}
	else if(Freq < 150000)
	{
		Freq <<= 1;

		HMC_Set_FreqRange(en,HMC_VCO_DIV2);
	}
	else if(Freq <= 300000)
	{
		HMC_Set_FreqRange(en,HMC_VCO_DIV0);
	}
	else
	{
		Freq >>= 1;

		HMC_Set_FreqRange(en,HMC_VCO_DOUBLE);
	}
	
	N = Freq / 1000;
	Frac = Freq % 1000;
	Frac *= HMC_FRAC_CONST_FULL;
	Frac /= 1000;
	
	Write_HMC(en, HMC_REG_NDIV, N);
	Write_HMC(en, HMC_REG_FRACDIV, (uint32_t)Frac);
	
}

void Write_HMC(gpio_pin_t en, uint8_t Register, uint32_t Value)
{
	uint8_t buf[4];
	
	Spi_Set_Dataorder(SPI_MSB_First);
	Spi_Set_Mode(SPI_Mode0);
	
	_delay_ms(1);
	
	//Address 7Bit
	Register &= 0x3F;
	Register <<= 1;
	
	//Data 24Bit
	Value &= 0xFFFFFF;
	Value <<= 1;
	
	if(Value & 0x1000000)
	{
		Register += 1;
	}

	buf[0] = Register;
	buf[1] = (Value & 0xFF0000) >> 16;
	buf[2] = (Value & 0xFF00) >> 8;
	buf[3] = (Value & 0xFF);
	
	Set_gpio_Output_High(en.gpio_struct, en.pinnumber);
	
	Spi_Send_Data(buf,4);
	
	Set_gpio_Output_Low(en.gpio_struct, en.pinnumber);
}

void Write_HMC_VCO(gpio_pin_t en, uint8_t Register, uint32_t Value)
{
	Value &= 0x1FF;
	Register &= 0xF;
	
	Value <<= 4;
	Value |= Register;
	
	Value <<= 3;

	Write_HMC(en, HMC_REG_VCO, Value);
}