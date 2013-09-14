#ifndef _AXON_H_
#define _AXON_H_

// Call this once after initializations is done
void AxonInit();

void AxonCommand(char command);
void AxonCommandWriteRegister(char reg, char val);

// Call this when SPI is ready
void OnAxonByteSent();

// Do not call these methods
void AxonProceed();
void AxonUpdate();
void AxonWriteByteToSend(char data);
void AxonStartCsn();
void AxonEndCsn();

typedef enum
{
	EAxonIdle = 0,	// Transmitter is ready
	EAxonBusy	
}	EAxonState;
extern char s_axonState;

#endif //_AXON_H_