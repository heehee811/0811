/*
 * board.h
 *
 * Created: 2022-07-17 오전 1:22:55
 *  Author: jjh83
 */ 


#ifndef BOARD_H_
#define BOARD_H_

//GPIOB
#define Spi_Pin_SS			0
#define Spi_Pin_SCK     1
#define Spi_Pin_MOSI    2
#define Spi_Pin_MISO    3
#define Fan_Pin_Enable	5
#define Hpa_Vgs_Enable	6
#define Hpa_Drv_Enable	7

//GPIOC
#define Clcd_Pin_RS			0
#define Clcd_Pin_RW			1
#define Clcd_Pin_EN			2
#define PLL_Pin_LO_LD		3
#define PLL_Pin_RF_LD		4
#define LED_Pin_RED		  5
#define LED_Pin_GREEN	  6
#define ADC_Pin_Select	7

//GPIOD
#define RS485_EN				1
#define Fan_Alarm1_Pin	4
#define Fan_Alarm2_Pin	5
#define Fan_Alarm_Clr		6
#define Hpa_Vds_Enable	7

//GPIOE
#define BTN_Pin_0				2
#define BTN_Pin_1				3
#define BTN_Pin_2				4
#define ENCODER_Pin_BTN	5
#define ENCODER_Pin_A		6
#define ENCODER_Pin_B		7

//GPIOG
#define SPI_En_DSA			0
#define SPI_En_VVA			1
#define SPI_En_PLL_RF		2
#define SPI_EN_PLL_LO		3
#define SPI_EN_ADC			4

#endif /* BOARD_H_ */