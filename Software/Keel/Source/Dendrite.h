#ifndef _DENDRITE_H_
#define _DENDRITE_H_

typedef enum
{
	ERxPowerUp0 = 0,
	ERxPowerUp1,
	ERxInterruptEnable0,
	ERxInterruptEnable1,
	ERxModeEnable0,
	ERxModeEnable1,
	ERxReady
}	ERxInitState;
extern char s_rxInitState;

typedef enum
{
	EDendriteInit			= 0,	// Receiver is not ready
	EDendriteIdle			= 1,	// Can check if data is ready to 
	EDendriteSendingRequest = 2,	// Send request to receive data
	EDendriteRead0			= 3,	// Data is being read
	EDendriteRead31			= 3+31,
	EDendriteFinish
	
}	EDendriteState;
extern char s_dendriteState;

void UpdateDendriteState();
void DendriteSendReadRequest();

#endif //_DENDRITE_H_