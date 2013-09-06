#include "Axon.h"

#include <avr/io.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"
#include "utils.h"
#include "TransmitPool.h"
#include "StreamBuffer.h"

extern TStreamBuffer s_stream;
char s_axonState = EAxonIdle;
unsigned short s_rxInitState = 0;
TPool s_dendrideActivePool = {0, 0};

#define AXON_INIT_SEQUENCE_SIZE 7
const char sk_axonInitSequence[AXON_INIT_SEQUENCE_SIZE] =
{
	0,
	// Power Up
	W_REGISTER | CONFIG,	
	1 << PWR_UP | 1 << EN_CRC,
	
	// Set frequency
	W_REGISTER | RF_CH,
	/*RADIO_FREQUENCY*/0,
	
	// Set data rate to 1 MHz
	W_REGISTER | RF_SETUP,
	0b00000111,	
};

const char sk_axonReadRequest[1] = 
{
	R_RX_PAYLOAD
};


void AxonInit()
{
	AxonAdvanceToState(EAxonInit);
	SET_BIT(PORTB, CSN);
}


void AxonAdvanceState()
{	
	char nextState = s_axonState + 1;
	if(nextState > EAxonFinish)
		nextState = EAxonIdle;
		
	AxonAdvanceToState(nextState);
}

void AxonAdvanceToState(char state)
{/*
	switch (state)
	{
		case EAxonInit:
			if(!IsDone(&s_dendrideActivePool))
				return;
			
			s_axonState = state;
			s_dendrideActivePool.pointer = sk_axonInitSequence;
			s_dendrideActivePool.size = AXON_INIT_SEQUENCE_SIZE;
		break;
		
		case EAxonIdle:
			s_axonState = state;
		break;
		
		case EAxonSendingRequest:
			if(!IsDone(&s_dendrideActivePool))
				return;
		
			s_axonState = state;
			s_dendrideActivePool.pointer = sk_axonReadRequest;
			s_dendrideActivePool.size = 1;
		break;
		
		case EAxonReadFirst:
		case EAxonReadLast:
		default:
			AxonSaveByte();
			s_axonState = state;
		break;
		
		case EAxonFinish:
			if(!IsDone(&s_dendrideActivePool))
				return;
			
			s_axonState = state;
			s_dendrideActivePool.pointer = sk_denditeFinishReadingSequence;
			s_dendrideActivePool.size = 2;
		break;
	}*/
}

void OnAxonInterrupt()
{
	if(s_axonState == EAxonIdle)
	{
		//AxonAdvanceToState(EAxonSendingRequest);
	}
}

void OnAxonSpiReady()
{
	SET_BIT(PORTB, CSN);
	if(s_axonState == EAxonIdle)
	{
		return;
	}
	else if(s_axonState == EAxonInit || s_axonState == EAxonSending || s_axonState == EAxonFinish)
	{
		if(IsDone(&s_dendrideActivePool))
		{
			AxonAdvanceState();
		}
		else
		{
			AxonPrepareNextByte();
		}
	}
	else if(s_axonState >= EAxonSendFirst && s_axonState <= EAxonSendLast)
	{
		AxonAdvanceState();
	}
}

void AxonPrepareNextByte()
{
	CLEAR_BIT(PORTB, CSN);
//	WRITE_REG(USIDR, PoolPopFront(&s_dendrideActivePool));

}

void AxonSaveByte()
{
//	WriteStream(&s_stream, USIDR);
}
