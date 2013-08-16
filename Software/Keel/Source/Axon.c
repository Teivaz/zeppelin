#include "Axon.h"
#include <avr/io.h>
#include "config.h"
#include "types.h"
#include "StreamBuffer.h"
#include "main.h"

char s_axonState = EAxonHalt;
char s_axonData = 0;

void UpdateAxonState()
{
	switch (s_axonState)
	{
	case EAxonHalt:
	// This status should remove configure
		break;
		
	case EAxonReady:
		// Check if has something to send
		if(GetStreamBufferSize(&s_stream) != 0)
		{
			AxonSend(ReadStream(&s_stream));
		}
		break;
		
	default:
		break;
	}
}

void AxonSend(char data)
{
	s_axonData = data;
	s_axonState = EAxonSending0;
}

void AxonIncrementBit()
{
	if(s_axonState >= EAxonSending0)
	{
		if(s_axonState > EAxonSending7)
		{
			s_axonState = EAxonReady;
			return;
		}
		
		(0x80 & s_axonData) ? SET_BIT(PORTB, AXON_MOSI) : CLEAR_BIT(PORTB, AXON_MOSI);
		s_axonData = s_axonData << 1;
		++s_axonState;
	}
}
