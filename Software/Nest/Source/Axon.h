#ifndef _AXON_H_
#define _AXON_H_

// Call this once after initializations is done
void AxonInit();

void AxonSend(char* data, char size);

void AxonStreamWrite(char data);
void AxonProceed();

void AxonWriteRegister(char reg, char val);
void AxonReadRegister(char reg);
void AxonCommand(char command);

// Do not call these methods
void AxonWriteByteToSend(char data);
void AxonStartCsn();
void AxonEndCsn();

#endif //_AXON_H_