#include "Axon.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"
#include "utils.h"
#include "StreamBuffer.h"

TStreamBuffer s_axonCommandStream;
char s_axonState = EAxonIdle;
	

void AxonInit()
{
	AxonEndCsn();
	InitializeStream(&s_axonCommandStream);
}

void AxonCommandWriteRegister(char reg, char val)
{
	cli();
	WriteStream(&s_axonCommandStream, W_REGISTER | reg);
	WriteStream(&s_axonCommandStream, val);
	AxonUpdate();
	sei();
}

void AxonCommand(char command)
{
	cli();
	WriteStream(&s_axonCommandStream, command);
	AxonUpdate();
	sei();
}

void AxonUpdate()
{
	if(s_axonState == EAxonIdle)
	{
		AxonProceed();
	}
}

void AxonProceed()
{
	switch (s_axonState)
	{
		case EAxonIdle:
			if(GetStreamBufferSize(&s_axonCommandStream))
			{
				AxonStartCsn();
				AxonWriteByteToSend(ReadStream(&s_axonCommandStream));
			}
			else
			{
				AxonEndCsn();
			}
		break;
		case EAxonBusy:
			;
		break;
	}
}

void OnAxonByteSent()
{
	s_axonState = EAxonIdle;
	AxonProceed();
}

void AxonWriteByteToSend(char data)
{
	WRITE_REG(SPDR, data);
}

void AxonStartCsn()
{
	CLEAR_BIT(PORTB, CSN);
}

void AxonEndCsn()
{
	SET_BIT(PORTB, CSN);
}
