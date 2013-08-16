#ifndef _AXON_H_
#define _AXON_H_

typedef enum
{
	EAxonHalt = 0,		// No sending allowed
	EAxonReady,			// Ready to send 
	EAxonSending0,		// Sending in progress
	EAxonSending1,
	EAxonSending2,
	EAxonSending3,
	EAxonSending4,
	EAxonSending5,
	EAxonSending6,
	EAxonSending7
}	EAxonState;

extern char s_axonState;
extern char s_axonData;

void UpdateAxonState();
void AxonIncrementBit();

void AxonSend(char data);
void UpdateDendriteState();
void DendriteSendReadRequest();

#endif //_AXON_H_