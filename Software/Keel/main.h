#ifndef _MAIN_H_
#define _MAIN_H_
int main();

typedef enum
{
	EAxonHalt = 0,		// No sending allowed
	EAxonReadyToSend,	// Ready to send 
	EAxonSending		// Sending in progress
}	EAxonState;
extern unsigned char s_axonState = EAxonHalt;

void UpdateAxonStatus(unsigned char state);
void AxonSend(unsigned char data);

typedef enum
{
	EDendriteHalt = 0,	// Receiver is not ready
	EDendriteIdle,		// Can receive data
	EDendriteReceiving,	// Data receiving in progress
	EDendriteReady		// Data should be processed and can not be received
}	EDendriteState;
extern unsigned char s_dendriteState = EDendriteIdle;

void UpdateDendriteStatus(unsigned char state);

extern unsigned char s_buffer = 0;


#endif //_MAIN_H_