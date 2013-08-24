#ifndef _DENDRITE_H_
#define _DENDRITE_H_

#define DENDRITE_PACKAGE_SIZE 32

// Call this when SPI is ready
void OnDendriteSpiReady();

// Call this method when interrupted by transceiver
void OnDendriteInterrupt();

// Do not call these methods
void DendritePrepareNextByte();
void DendriteAdvanceState();
void DendriteAdvanceToState(char state);
void DendriteSaveByte();

typedef enum
{
	EDendriteInit			= 0,	// Receiver is not ready
	EDendriteIdle			= 1,	// Can check if data is ready to 
	EDendriteSendingRequest = 2,	// Send request to receive data
	EDendriteReadFirst		= 3,	// Data is being read
	EDendriteReadLast		= 3 + DENDRITE_PACKAGE_SIZE - 1,
	EDendriteFinish,
}	EDendriteState;
extern char s_dendriteState;


#endif //_DENDRITE_H_