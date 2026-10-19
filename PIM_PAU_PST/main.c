/*
 * PIM_PAU_PST.c
 *
 * Created: 2022-07-16 오후 7:28:08
 * Author : jjh83
 */

#include "main.h"

Pau_Status_t* system_status_ptr; //포인터.
Pau_Setting_t* system_setting_ptr;

//MA_t FWD_ADC_MA;
MA_t RVS_ADC_MA;
MA_t TEMP_ADC_MA;

int8_t DSA_BACKUP;

int main(void)
{
    /* Replace with your application code */
    Init_System();	//Init = 초기화. 초기화 시스템
		system_status_ptr->Power = 200;
    while (1)	//반복
    {			
			Task_Packet();	//데이터 묶음
			Task_Atten(system_status_ptr,system_setting_ptr);	//감쇄기
			Task_Encoder(system_status_ptr,system_setting_ptr);	//엔코더 (장비에 달린 다이얼)
			Task_Detector(system_status_ptr,system_setting_ptr);	//디렉터 (RF 신호의 크기(전력)를 감지하는 센서
			Task_VSWR(system_status_ptr,system_setting_ptr);	//안테나와 케이블 매칭을 나타냄
			Task_ALC(system_status_ptr,system_setting_ptr);		//출력 신호의 크기가 항상 일정한 크기를 유지하도록 잡아주는 기능
			Task_Display(system_status_ptr,system_setting_ptr);	//화면 출력 장치
			Task_RFPath(system_status_ptr);		//내부 회로 통로 (스위치 경로)
			Task_FAN(system_status_ptr);		//냉각팬
			Task_Indicator(system_status_ptr);	//LED 또는 부저
			Task_Button();
			//Repeat Task Every 50ms
			if((Get_Systick() % 100) == 0)	//만약 Get_Systick 
			{
				//Display_Changed();
				Task_PLL_Lockdetect(system_status_ptr);
				Task_Temp(system_status_ptr);
			}
			
			Set_gpio_Pin_Output_Toggle(gpio_fan_alarm_clear);
    }
}

void Task_Packet()
{
	uint8_t buf;	//상태 체크
	Pau_Status_t* bbb = Get_Status();	//시스템 상태를 가져올 포인터 bbb 선언
	
	if(Get_PacketStatus() != false)	//Get_PacketStatus 가 false 가 아니라면 이번 주기는 리턴
	{
		return;
	}
	
	else if(Uart_Rx_DTR() == 0)	//Uart_Rx_DTR에 신호가 없다면 리턴
	{
		return;
	}
	
	else 
	{
		while(Uart_Rx(&buf) != false)	//false이 아닌 상태라면 1바이트씩 상태를 계속 읽어옴
		{
			if(Analyze_Packet(buf) != false)	//1바이트씩 모여서 패킷이 완성 됐는 지 확인
			{
				if(Compare_Address() != false)	//주소 확인
				{
					Comm_Handler();	//처리 함수 호출
				}
				else
				{
					Init_PacketReceive();	//주소가 확인되지 않았다면 다시 읽어오기
				}
			}
		}
	}
}

void Task_Atten(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr)
{
	static uint8_t dsa_;
	static uint16_t vva_;
	
	static uint32_t Freq_ = 0;
	static uint16_t Power_ = 0;
	
	if(Power_ != stat_ptr->Power)	//목표 전력이 변경되었을 때 사용됨.
	{
		if(set_ptr->Funcs.ALC_Enable == false)	//ALC 가 꺼져있는 경우에만 실행
		{
			stat_ptr->VVA_ATT = Get_Detec_Table()->Table[stat_ptr->Power - Start_Power + stat_ptr->Delta].Atten;
		}
		
		if(stat_ptr->Power < 300)
		{
			set_ptr->DSA_Preset = 30;
		}
		else
		{
			set_ptr->DSA_Preset = 10;
		}
		
		Power_ = stat_ptr->Power;
	}

	if(dsa_ != set_ptr->DSA_Preset)
	{
		if(set_ptr->DSA_Preset > 30)
		{
			set_ptr->DSA_Preset = 30;
		}
		else if(set_ptr->DSA_Preset < 0)
		{
			set_ptr->DSA_Preset = 0;
		}
		
		Bva_Set_ATT(gpio_bva_en, 0x00, set_ptr->DSA_Preset);
		dsa_ = set_ptr->DSA_Preset;
	}
	
	if(vva_ != stat_ptr->VVA_ATT)
	{
		if(stat_ptr->VVA_ATT > 4090)
		{
			stat_ptr->VVA_ATT = 4090;
		}
		else if(stat_ptr->VVA_ATT < 0)
		{
			stat_ptr->VVA_ATT = 0;
		}
		Set_DAC(gpio_vva_en,stat_ptr->VVA_ATT);
		vva_ = stat_ptr->VVA_ATT;
	}
	
	if(Freq_ != stat_ptr->Frequency)
	{
		stat_ptr->Delta = Get_DeltaPower(stat_ptr->Frequency);
		Set_HMC_Frequency(gpio_rfpll_en, stat_ptr->Frequency);
		HMC_RF_Enable(gpio_lopll_en, true);
		Set_HMC_Frequency(gpio_lopll_en, stat_ptr->Frequency+16180);
		
		Freq_ = stat_ptr->Frequency;
	}
}

void Task_Encoder(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr)
{
	int16_t Enc = Get_Encoder();
	uint8_t Encbtn = Get_EncoderButton();
	if(Enc == 0)
	{
		return;
	}
	
	switch(stat_ptr->Status)
	{
		case SYSTEM_Setting:
			set_ptr->Module_Address += Enc;
			Save_System_Settings();
			break;
		case SYSTEM_Tuning:
			if(Enc > 0)
			{
				stat_ptr->VVA_ATT += 2;
			}
			else
			{
				stat_ptr->VVA_ATT -= 2;
			}
			break;
		
		case SYSTEM_RFOUT:
		case SYSTEM_IDLE:
		if(Encbtn != false)
		{
			stat_ptr->SysFlag.Encflag ^= 1;
			Clr_EncoderButton();
		}
		if(stat_ptr->SysFlag.Encflag != false)
		{
			if(Enc > 0)
			{
				stat_ptr->Frequency += 10;
			}
			else
			{
				stat_ptr->Frequency -= 10;
			}
		}
		else
		{
			if(Enc > 0)
			{
				stat_ptr->Power += 1;
			}
			else
			{
				stat_ptr->Power -= 1;
			}
		}

		break;
	}
	Clear_Encoder();
}

void Task_Detector(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr)
{
	static uint8_t sw = false;
	uint16_t buff = 0;
	
	if(stat_ptr->SysFlag.RFFlag == 0)
	{
		return;
	}
	
	if(stat_ptr->SysFlag.DetFlag == 0)
	{
		sw ^= 1;
		Set_gpio_Pin_Output(gpio_adc_rvsfwd,sw);
		_delay_us(500);
	}
	else
	{
		sw = false;
		Set_gpio_Pin_Output(gpio_adc_rvsfwd,sw);
	}
	
	
	if(sw == false)
	{
		stat_ptr->FWD_ADC = Get_ADS7886(gpio_adc_en);
		system_status_ptr->FWD = Get_DetectPower(system_status_ptr->FWD_ADC);
		system_status_ptr->FWD += set_ptr->Display_Offset - stat_ptr->Delta;
	}
	else
	{
		stat_ptr->RVS_ADC = Push_MA(&RVS_ADC_MA,Get_ADS7886(gpio_adc_en));
		system_status_ptr->RVS = Get_DetectPower(system_status_ptr->RVS_ADC);
		system_status_ptr->RVS += set_ptr->Display_Offset - stat_ptr->Delta;;
	}
	
	system_status_ptr->RL = system_status_ptr->FWD - system_status_ptr->RVS;
}

void Task_VSWR(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr)
{
	if(stat_ptr->SysFlag.RFFlag == false)
	{
		return;
	}
	
	if(stat_ptr->FWD < 330)
	{
		return;
	}
	
	if(set_ptr->Funcs.SWR_Shutdown_Enable == false)
	{
		return;
	}
	
	if(stat_ptr->RL < VSWR_Threshold)
	{
		stat_ptr->Alarm = VSWR_Alarm;
	}
}

void Task_ALC(Pau_Status_t* stat_ptr, Pau_Setting_t* set_ptr)
{
	if(set_ptr->Funcs.ALC_Enable == false)
	{
		return;
	}
	if(stat_ptr->SysFlag.RFFlag == false)
	{
		return;
	}
	if(stat_ptr->Status != SYSTEM_RFOUT)
	{
		return;
	}
	
	int16_t dev = stat_ptr->FWD - stat_ptr->Power + set_ptr->Power_Offset;
	//dev += ALC_Delta;
	
	stat_ptr->VVA_ATT -= dev;
	
	if(set_ptr->Funcs.ALC_Shutdown_Enable == false)
	{
		return;
	}
	
	if(stat_ptr->VVA_ATT > 4090)
	{
		stat_ptr->Alarm = ALC_Shutdown;
		return;
	}
	
	if(stat_ptr->VVA_ATT < 0)
	{
		stat_ptr->Alarm = ALC_Shutdown;
		return;
	}
}

void Task_RFPath(Pau_Status_t* stat_ptr)
{
	static uint8_t status_ = SYSTEM_IDLE;

	if(status_ != stat_ptr->Status)
	{
		switch(stat_ptr->Status)
		{
			case SYSTEM_IDLE:
			case SYSTEM_Setting:
			case SYSTEM_Alarm:
				HMC_RF_Enable(gpio_lopll_en, false);
				HMC_RF_Enable(gpio_rfpll_en, false);
				Set_gpio_Pin_Output_Low(gpio_hpa_vds);
				stat_ptr->SysFlag.RFFlag = false;
				break;
			
			case SYSTEM_RFOUT:
			case SYSTEM_Tuning:
				HMC_RF_Enable(gpio_rfpll_en, true);
				Set_HMC_Frequency(gpio_rfpll_en, stat_ptr->Frequency);
				
				HMC_RF_Enable(gpio_lopll_en, true);
				Set_HMC_Frequency(gpio_lopll_en, stat_ptr->Frequency+16180);
				Set_gpio_Pin_Output_High(gpio_hpa_vds);
				stat_ptr->SysFlag.RFFlag = true;
				break;
		}

		status_ = stat_ptr->Status;
	}
}

void Task_PLL_Lockdetect(Pau_Status_t* stat_ptr)
{
	static uint8_t rf_fail_counter = 0;
	static uint8_t lo_fail_counter = 0;
	
	stat_ptr->SysFlag.RfLDFlag = Get_gpio_Pin_Input(gpio_rfpll_ld);
	stat_ptr->SysFlag.LoLDFlag = Get_gpio_Pin_Input(gpio_lopll_ld);
	
	
	if(stat_ptr->SysFlag.RFFlag == false)
	{
		rf_fail_counter = 0;
		lo_fail_counter = 0;
		return;
	}
	
	if(stat_ptr->Status != SYSTEM_RFOUT)
	{
		return;
	}
	
	if(stat_ptr->SysFlag.RfLDFlag == false)
	{
		++rf_fail_counter;
		return;
	}
	
	if(stat_ptr->SysFlag.LoLDFlag == false)
	{
		++lo_fail_counter;
		return;
	}
	
	if(rf_fail_counter > 10)
	{
		stat_ptr->Alarm = PLL_Fail;
	}
	
	else if(lo_fail_counter > 10)
	{
		stat_ptr->Alarm = Low_Power;
	}
}

void Task_Temp(Pau_Status_t* stat_ptr)
{
	Start_ADC();
	
	stat_ptr->Temp = Push_MA(&TEMP_ADC_MA,Get_ADC_Result(0) - 500);
	
	if(Get_System_Mode() != SYSTEM_RFOUT)
	{
		return;
	}

	if(stat_ptr->Temp > System_Temp_Limit)
	{
		stat_ptr->Alarm = Over_Temp;
	}
}

void Task_FAN(Pau_Status_t* stat_ptr)
{
	if(stat_ptr->Temp >= System_Fan_High_Threshold)
	{
		stat_ptr->SysFlag.FanFlag = true;
	}
	if(stat_ptr->Temp <= System_Fan_Low_Threshold)
	{
		stat_ptr->SysFlag.FanFlag = false;
	}
	
	if(stat_ptr->SysFlag.FanFlag != false)
	{
		Set_gpio_Pin_Output_High(gpio_fan);
	}
	else
	{
		Set_gpio_Pin_Output_Low(gpio_fan);
	}
}

void Task_Indicator(Pau_Status_t* stat_ptr)
{
	if(stat_ptr->Alarm != Normal)
	{
		Set_System_Mode(SYSTEM_Alarm);
		Set_gpio_Pin_Output_High(gpio_led_red);
	}
	else
	{
		Set_gpio_Pin_Output_Low(gpio_led_red);
	}
	
	if(stat_ptr->SysFlag.RFFlag != false)
	{
		Set_gpio_Pin_Output_High(gpio_led_green);
	}
	else
	{
		Set_gpio_Pin_Output_Low(gpio_led_green);
	}
}

void Task_Button()
{
	static uint8_t prev_btn = 0;
	uint8_t btn = 0;
	if(Get_gpio_Pin_Input(gpio_btn0) == 0)
	{
		btn = 2;
	}
	
	else if(Get_gpio_Pin_Input(gpio_btn1) == 0)
	{
		btn = 1;
	}
	
	else
	{
		btn = 0;
	}
	
	if(prev_btn != btn)
	{
		if(btn == 1)
		{
			if(Get_System_Mode() == SYSTEM_Setting)
			{
				Set_gpio_Pin_Output_High(gpio_hpa_vds);
			}
			else if(Get_System_Mode() == SYSTEM_IDLE)
			{
				Set_System_Mode(SYSTEM_RFOUT);
			}
			else
			{
				Set_System_Mode(SYSTEM_IDLE);
			}
		}
		
		else if(btn == 2)
		{
			if(Get_System_Mode() == SYSTEM_IDLE)
			{
				Set_System_Mode(SYSTEM_Setting);
			}
			
			else if(Get_System_Mode() == SYSTEM_Setting)
			{
				Set_System_Mode(SYSTEM_IDLE);
			}
			
			else if(Get_System_Mode() == SYSTEM_Tuning)
			{
				system_status_ptr->SysFlag.DetFlag = true;
			}
		}
		
		prev_btn = btn;
	}
}

void Init_System()
{
	Pau_Status_t* Stat = Get_Status();
	Pau_Setting_t* set = Get_Settings();
	set->DSA_Preset = 30;
	Stat->VVA_ATT = 0;
	Stat->Status = SYSTEM_IDLE;
	
	ATOMIC_BLOCK(ATOMIC_FORCEON)	//아토믹 블럭은 시작할 때 인터럽트를 끔.
	{
		wdt_disable();
		_delay_ms(1000);
		system_status_ptr = Get_Status();
		system_setting_ptr = Get_Settings();
		Init_Port();
		Init_SPI(gpiob, Spi_Pin_SS,Spi_Pin_SCK,Spi_Pin_MOSI, Spi_Pin_MISO);
		Init_Display();

		Init_ADC();
		ADC_Set_Channel(0);
		Init_EINT();
		
		Init_UART(115200UL,100);
		
		Load_System_Settings();
		
		Init_Detection_Table();
		Init_Delta_Table();
		
		Init_HMC(gpio_rfpll_en);
		Init_HMC(gpio_lopll_en);
		HMC_RF_Enable(gpio_rfpll_en, false);
		
		HMC_RF_Enable(gpio_lopll_en, false);
		
		
		
		Init_Comm(gpio_485_en);
		
		printf("Complete!");
		_delay_ms(1000);
		
		DSA_BACKUP = set->DSA_Preset;
		Bva_Set_ATT(gpio_bva_en,0, set->DSA_Preset);
		Set_DAC(gpio_vva_en, Stat->VVA_ATT);
		
		Clcd_Command(CMD_DISPLAY_CLR);
		
		Set_System_Mode(SYSTEM_IDLE);

		Init_Systick();
	}
}

void Init_Port()
{
	
	DDRG = 0x1F;	//G포트의 일정 부분을 출력으로 사용하겠다
	PORTG = 0x1F;	//처음 시작부터 출력으로 사용될 일정 부분을 1로 두고 시작
	//OUTPUT
	//PORTB
	gpio_fan.gpio_struct = gpiob;
	gpio_fan.pinnumber = Fan_Pin_Enable;
	Set_gpio_Pin_Direction_Output(gpio_fan);
	Set_gpio_Pin_Output_High(gpio_fan);
	
	gpio_hpa_drv.gpio_struct = gpiob;
	gpio_hpa_drv.pinnumber = Hpa_Drv_Enable;
	Set_gpio_Pin_Direction_Output(gpio_hpa_drv);
	Set_gpio_Pin_Output_High(gpio_hpa_drv);
	
	gpio_hpa_vgs.gpio_struct = gpiob;
	gpio_hpa_vgs.pinnumber = Hpa_Vgs_Enable;
	Set_gpio_Pin_Direction_Output(gpio_hpa_vgs);
	Set_gpio_Pin_Output_High(gpio_hpa_vgs);
	//PORTC
	gpio_adc_rvsfwd.gpio_struct = gpioc;
	gpio_adc_rvsfwd.pinnumber = ADC_Pin_Select;
	Set_gpio_Pin_Direction_Output(gpio_adc_rvsfwd);
	Set_gpio_Pin_Output_Low(gpio_adc_rvsfwd);
	
	gpio_led_green.gpio_struct = gpioc;
	gpio_led_green.pinnumber = LED_Pin_GREEN;
	Set_gpio_Pin_Direction_Output(gpio_led_green);
	Set_gpio_Pin_Output_High(gpio_led_green);
	
	gpio_led_red.gpio_struct = gpioc;
	gpio_led_red.pinnumber = LED_Pin_RED;
	Set_gpio_Pin_Direction_Output(gpio_led_red);
	Set_gpio_Pin_Output_High(gpio_led_red);
	//PORTD
	gpio_485_en.gpio_struct = gpiod;
	gpio_485_en.pinnumber = RS485_EN;
	Set_gpio_Pin_Direction_Output(gpio_485_en);
	Set_gpio_Pin_Output_Low(gpio_485_en);
	
	gpio_hpa_vds.gpio_struct = gpiod;
	gpio_hpa_vds.pinnumber = Hpa_Vds_Enable;
	Set_gpio_Pin_Direction_Output(gpio_hpa_vds);
	Set_gpio_Pin_Output_Low(gpio_hpa_vds);
	
	gpio_fan_alarm_clear.gpio_struct = gpiod;
	gpio_fan_alarm_clear.pinnumber = Fan_Alarm_Clr;
	Set_gpio_Pin_Direction_Output(gpio_fan_alarm_clear);
	Set_gpio_Pin_Output_Low(gpio_fan_alarm_clear);
	//PORTG
	gpio_bva_en.gpio_struct = gpiog;
	gpio_bva_en.pinnumber = SPI_En_DSA;
	
	gpio_vva_en.gpio_struct = gpiog;
	gpio_vva_en.pinnumber = SPI_En_VVA;
	
	gpio_rfpll_en.gpio_struct = gpiog;
	gpio_rfpll_en.pinnumber = SPI_En_PLL_RF;
	
	gpio_lopll_en.gpio_struct = gpiog;
	gpio_lopll_en.pinnumber = SPI_EN_PLL_LO;
	
	gpio_adc_en.gpio_struct = gpiog;
	gpio_adc_en.pinnumber = SPI_EN_ADC;
	//INPUT
	//PORTC
	gpio_rfpll_ld.gpio_struct = gpioc;
	gpio_rfpll_ld.pinnumber = PLL_Pin_RF_LD;
	Set_gpio_Pin_Direction_Input(gpio_rfpll_ld);
	Set_gpio_Pin_Output_Low(gpio_rfpll_ld);
	
	gpio_lopll_ld.gpio_struct = gpioc;
	gpio_lopll_ld.pinnumber = PLL_Pin_LO_LD;
	Set_gpio_Pin_Direction_Input(gpio_lopll_ld);
	Set_gpio_Pin_Output_Low(gpio_lopll_ld);
	//PORTD
	gpio_fan_alarm1.gpio_struct = gpiod;
	gpio_fan_alarm1.pinnumber = Fan_Alarm1_Pin;
	Set_gpio_Pin_Direction_Input(gpio_fan_alarm1);
	Set_gpio_Pin_Output_Low(gpio_fan_alarm1);
	
	gpio_fan_alarm2.gpio_struct = gpiod;
	gpio_fan_alarm2.pinnumber = Fan_Alarm2_Pin;
	Set_gpio_Pin_Direction_Input(gpio_fan_alarm2);
	Set_gpio_Pin_Output_Low(gpio_fan_alarm2);
	
	gpio_btn0.gpio_struct = gpioe;
	gpio_btn0.pinnumber = BTN_Pin_0;
	Set_gpio_Pin_Direction_Input(gpio_btn0);
	Set_gpio_Pin_Output_High(gpio_btn0);
	
	gpio_btn1.gpio_struct = gpioe;
	gpio_btn1.pinnumber = BTN_Pin_1;
	Set_gpio_Pin_Direction_Input(gpio_btn1);
	Set_gpio_Pin_Output_High(gpio_btn1);
}

void Init_EINT()
{
	//Falling Edge Interrupt (For OFF)
	EICRB |= (1<<ISC61) | (1<<ISC51) | (1<<ISC41);
	EIFR = 0xFF;
	//Interrupt Enable
	EIMSK = (1<<INT6) | (1<<INT5) | (1<<INT4);
}

void Init_TIM()
{
	ICR1 = 1000;
	//Timer Top
	TCCR1A = _BV(COM1A1) | _BV(WGM11);
	TCCR1B = _BV(WGM13) | _BV(CS11) | _BV(CS10);
	//Phase correct PWM, 64 Prescale

	TCCR2 = _BV(WGM21) | _BV(WGM20) | _BV(CS22);
	//Fast PWM, 256 Prescale

	//Communication Counter
	ICR3 = 2000;
	TCNT3 = 0;
	OCR1A = 2000;

	TCCR3A = _BV(WGM31);
	TCCR3B = _BV(WGM32) | _BV(WGM33) | _BV(CS30);
	// Fast PWM, Fundamental Clock

	TIFR = 0xFF;
	TIMSK |= _BV(TOIE2);
}

void Init_Display()
{
	Clcd_Open(gpioc, gpioa, 0, 1, 2);
	Clcd_Init();
	
	Clcd_Command(CMD_DISPLAY_CLR);
	
	Clcd_Command(LINE1);
	printf("Innertron ActiveTeam");
	Clcd_Command(LINE2);
	printf("PIMA PAU Ver %01d.%02d.%02d",Version_Major,Version_Minor,Version_Patch);
	Clcd_Command(LINE3);
	printf("%s %s",__DATE__,__TIME__);
	Clcd_Command(LINE4);
	printf("Booting...");
}
