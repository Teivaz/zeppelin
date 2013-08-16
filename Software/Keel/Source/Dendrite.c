#include "Dendrite.h"

#include <avr/io.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"
#include "TransmitPool.h"
#include "StreamBuffer.h"
#include "SystemConfig.h"

extern TStreamBuffer s_stream;
char s_dendriteState = EDendriteIdle;
unsigned short s_rxInitState = 0;
TPool s_dendrideActivePool;

#define DENDRITE_INIT_SEQUENCE_SIZE 10
const char sk_dendriteInitSequence[DENDRITE_INIT_SEQUENCE_SIZE] =
{
	// Power Up
	W_REGISTER | CONFIG,	
	1 << PWR_UP | 1 << EN_CRC,
	
	// Enable interrupts
	W_REGISTER | CONFIG,
	1 << PWR_UP | 1 << EN_CRC | 1 << MASK_RX_DR,
	
	// Set frequency
	W_REGISTER | RF_CH,
	RADIO_FREQUENCY,
	
	// Set data rate to 1 MHz
	W_REGISTER | RF_SETUP,
	0b00000111,	
	
	// Enable receiver
	W_REGISTER | CONFIG,
	1 << PWR_UP | 1 << EN_CRC | 1 << MASK_RX_DR | 1 << PRIM_RX
};

const char sk_dendriteReadRequest[1] = 
{
	R_RX_PAYLOAD
};

const char sk_denditeFinishReadingSequence[2] = 
{
	W_REGISTER | STATUS,
	0
};

void DendriteAdvanceState()
{
	char nextState = s_dendriteState + 1;
	if(nextState > EDendriteFinish)
		nextState = EDendriteIdle;
		
	DendriteAdvanceToState(nextState);
}

void DendriteAdvanceToState(char state)
{
	switch (state)
	{
		case EDendriteInit:
			if(!IsDone(s_dendrideActivePool))
				return;
			
			s_dendriteState = state;
			s_dendrideActivePool.pointer = sk_dendriteInitSequence;
			s_dendrideActivePool.size = DENDRITE_INIT_SEQUENCE_SIZE;
		break;
		
		case EDendriteIdle:
			s_dendriteState = state;
		break;
		
		case EDendriteSendingRequest:
			if(!IsDone(s_dendrideActivePool))
				return;
		
			s_dendriteState = state;
			s_dendrideActivePool.pointer = sk_dendriteReadRequest;
			s_dendrideActivePool.size = 1;
		break;
		
		case EDendriteReadFirst:
		case EDendriteReadLast:
		default:
			DendriteSaveByte();
			s_dendriteState = state;
		break;
		
		case EDendriteFinish:
			if(!IsDone(s_dendrideActivePool))
				return;
			
			s_dendriteState = state;
			s_dendrideActivePool.pointer = sk_denditeFinishReadingSequence;
			s_dendrideActivePool.size = 2;
		break;
	}
}

void OnDendriteInterrupt()
{
	if(s_dendriteState == EDendriteIdle)
	{
		DendriteAdvanceToState(EDendriteSendingRequest);
	}
}

void OnDendriteSpiReady()
{
	if(s_dendriteState == EDendriteIdle)
		return;
	else if(s_dendriteState == EDendriteInit || s_dendriteState == EDendriteSendingRequest || s_dendriteState == EDendriteFinish)
	{
		if(IsDone(s_dendrideActivePool))
		{
			DendriteAdvanceState();
		}
		else
		{
			DendritePrepareNextByte();
		}
	}
	else if(s_dendriteState >= EDendriteReadFirst && s_dendriteState <= EDendriteReadLast)
	{
		DendriteAdvanceState();
	}
}

void DendritePrepareNextByte()
{
	WRITE_REG(USIDR, PoolPopFront(s_dendrideActivePool));
}

void DendriteSaveByte()
{
	WriteStream(&s_stream, USIDR);
}
