#include "Axon.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "StreamBuffer.h"
#include "nRF24L01.h"
#include "config.h"
#include "types.h"
#include "utils.h"

TStreamBuffer s_axonCommandStream;
	
void AxonStreamWrite(char data)
{
	WriteStream(&s_axonCommandStream, data);
}

void AxonInit()
{
	AxonEndCsn();
	InitializeStream(&s_axonCommandStream);
}

void AxonCommand2(char cmnd, char data)
{
	WriteStream(&s_axonCommandStream, cmnd);
	WriteStream(&s_axonCommandStream, data);
}

void AxonReadRegister( char reg )
{
	WriteStream(&s_axonCommandStream, R_REGISTER | reg);
	WriteStream(&s_axonCommandStream, 0xFF);
	AxonProceed();
}

void AxonWriteRegister(char reg, char val)
{
	WriteStream(&s_axonCommandStream, W_REGISTER | reg);
	WriteStream(&s_axonCommandStream, val);
	AxonProceed();
}

void AxonCommand(char command)
{
	WriteStream(&s_axonCommandStream, command);
	AxonProceed();
}

void AxonSend(char* data, char size)
{
	for(uint8_t a = 0; a < size; ++a)
	{
		WriteStream(&s_axonCommandStream, data[a]);
	}
	AxonProceed();
}

void AxonProceed()
{
	AxonStartCsn();
	while(GetStreamBufferSize(&s_axonCommandStream))
	{
		AxonWriteByteToSend(ReadStream(&s_axonCommandStream));
		while(!READ_BIT(SPSR, SPIF));
	}
	AxonEndCsn();
	
}

void AxonWriteByteToSend(char data)
{
	WRITE_REG(SPDR, data);
}

void AxonStartCsn()
{
	while(READ_BIT(PINB, CLK)); // Wait for CLK to lower
	CLEAR_BIT(PORTB, CSN);
}

void AxonEndCsn()
{
	SET_BIT(PORTB, CSN);
}
