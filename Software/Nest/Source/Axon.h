#ifndef _AXON_H_
#define _AXON_H_

#define AXON_PACKAGE_SIZE 33

// Call this once after initializations is done
void AxonInit();

// Call this when SPI is ready
void OnAxonSpiReady();

// Call this method when interrupted by transceiver
void OnAxonInterrupt();

// Do not call these methods
void AxonPrepareNextByte();
void AxonAdvanceState();
void AxonAdvanceToState(char state);
void AxonSaveByte();

typedef enum
{
	EAxonInit			= 0,	// Transmitter is not ready
	EAxonIdle			= 1,	// Does nothing
	EAxonSending		= 2,	// Send data
	EAxonSendFirst		= 3,	// Data is being read
	EAxonSendLast		= 3 + AXON_PACKAGE_SIZE,
	EAxonFinish,
}	EAxonState;
extern char s_axonState;

#endif //_AXON_H_