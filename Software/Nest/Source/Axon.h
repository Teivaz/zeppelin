#ifndef _AXON_H_
#define _AXON_H_

#define AXON_PACKAGE_SIZE 32

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
	EAxonInit			= 0,	// Receiver is not ready
	EAxonIdle			= 1,	// Can check if data is ready to 
	EAxonSendingRequest = 2,	// Send request to receive data
	EAxonReadFirst		= 3,	// Data is being read
	EAxonReadLast		= 3 + AXON_PACKAGE_SIZE,
	EAxonFinish,
}	EAxonState;
extern char s_axonState;

#endif //_AXON_H_