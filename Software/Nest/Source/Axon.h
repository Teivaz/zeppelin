#ifndef _AXON_H_
#define _AXON_H_

// Call this once after initializations is done
void AxonInit();

void AxonSend(char* data, char size);
void AxonWriteRegister(char reg, char val);
void AxonReadRegister(char reg);
void AxonCommand(char command);

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