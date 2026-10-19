/*
 * Uart_Device.c
 *
 * Created: 2022-07-17 오전 3:07:51
 *  Author: jjh83
 */ 
#include <Uart_Device.h>

//ringbuffer_t* txd;
ringbuffer_t* rxd;

//uint8_t txd_Busy;

int Ringbuffer_DataLeft(ringbuffer_t* Ringbuffer)
{
	//Normal
	if(Ringbuffer->input_p >= Ringbuffer->output_p)
	{
		return Ringbuffer->input_p - Ringbuffer->output_p;
	}
	//Data OVF
	else
	{
		return Ringbuffer->Length - Ringbuffer->output_p + Ringbuffer->input_p;
	}
}

uint8_t Ringbuffer_Tryget(ringbuffer_t* Ringbuffer, uint8_t* data)
{
	if(Ringbuffer_DataLeft(Ringbuffer)>0)
	{
		*data = Ringbuffer->Buffer[Ringbuffer->output_p++];
		if(Ringbuffer->output_p > Ringbuffer->Length)
				Ringbuffer->output_p = 0;

		return true;
	}
	else
		return false;
}

int Ringbuffer_Push(ringbuffer_t* Ringbuffer, uint8_t data)
{
	Ringbuffer->Buffer[Ringbuffer->input_p] = data;

	if(Ringbuffer->input_p++ >= Ringbuffer->Length)
			Ringbuffer->input_p = 0;
}

int Ringbuffer_Init(ringbuffer_t* Ringbuffer, int Length)
{
	Ringbuffer->Buffer = (uint8_t*)malloc(Length);
	Ringbuffer->Length = Length;
	Ringbuffer->input_p = 0;
	Ringbuffer->output_p = 0;
}

void RingBuffer_Flush(ringbuffer_t* Ringbuffer)
{
	Ringbuffer->input_p = 0;
	Ringbuffer->output_p = 0;
}

ISR(USART1_RX_vect)
{
	uint8_t data = UDR1;

	if(!(UCSR1A & ((1<<FE1)|(1<<DOR1))))
	{
		Ringbuffer_Push(rxd, data);
		//UDR1 = data;
	}
}

ISR(USART1_TX_vect)
{
// 	if(Ringbuffer_Tryget(txd,&UDR1) == false)
// 	{
// 		txd_Busy = false;
// 	}
}

void Init_UART(long baud, uint16_t max_recv_cnt)
{
	long Baudr = (F_CPU/(16*baud))-1;
	uint8_t UBRRH = Baudr>>8;
	uint8_t UBRRL = (uint8_t)Baudr&0x00ff;
	UBRR1H = UBRRH;
	UBRR1L = UBRRL;

	UCSR1A = 0x40;
	UCSR1C = 0x00;
	UCSR1C |= (1<<UCSZ01) | (1<<UCSZ00);
	UCSR1B = 0x00;
	UCSR1B |= (1<<RXEN0) | (1<<TXEN0);
	UCSR1B |= (1<<RXCIE0);
	
	rxd = (ringbuffer_t*)malloc(sizeof(ringbuffer_t));
	//txd = (ringbuffer_t*)malloc(sizeof(ringbuffer_t));

	//Ringbuffer_Init(txd, Uart_Txd_BufferSize);
	Ringbuffer_Init(rxd, Uart_Rxd_BufferSize);
	
	//txd_Busy = false;
}

uint8_t Uart_Txs(uint8_t* pkt,uint8_t size)
{
	while(size-- > 0)
	{
		UDR1 = *pkt++;
		while(!(UCSR1A & _BV(UDRE1)));
		//Ringbuffer_Push(txd,*(pkt++));
	}
	
// 	if(txd_Busy == false)
// 	{
// 		Ringbuffer_Tryget(txd,&UDR1);
// 		txd_Busy = true;
// 	}
}

uint8_t Uart_Tx(uint8_t data)
{
	UDR1 = data;
	while(!(UCSR1A & _BV(UDRE1)));
}

uint8_t Uart_Rx(uint8_t* data)
{
	return Ringbuffer_Tryget(rxd,data);
}

void Uart_Flush()
{
	RingBuffer_Flush(rxd);
}

uint16_t Uart_Rx_DTR()
{
	return Ringbuffer_DataLeft(rxd);
}