#include "Dendrite.h"

#include <avr/io.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"

char s_dendriteState = EDendriteIdle;
char s_dendriteInitState = 0;

const char sk_dendriteInitSequenceSize = 8;
volatile const char sk_dendriteInitSequence[8] =
{
	W_REGISTER | CONFIG,	
	1 << PWR_UP,			// Power Up
	W_REGISTER | CONFIG,
	1 << MASK_RX_DR,		// Enable interrupts
	W_REGISTER | RF_SETUP,
	FREQUENCY,				// Set frequency
	W_REGISTER | RF_SETUP,
	0b00000111//,
	
};

void UpdateDendriteState()
{
	switch (s_dendriteState)
	{
	case EDendriteInit:
		break;
	}
}

void AdvanceDendriteInit()
{
	if(s_dendriteInitState == sk_dendriteInitSequenceSize)
	WRITE_REG(USIDR, sk_dendriteInitSequence[s_rxInitState++]);
}

void DendriteSendReadRequest()
{
	WRITE_REG(USIDR, R_RX_PAYLOAD);
}
void DendriteSendFlushRequest()
{
}
