#ifndef _DENDRITE_H_
#define _DENDRITE_H_

// Call this once after initializations is done
void DendriteInit();

void DendriteWrite(char* data, char size);

void DendriteInterrupt();

void DendriteStart();
void DendriteEnd();

void DendriteRead();
void DendriteFinishReading();
void DendriteInitRx();

#define PDBG		CLEAR_BIT(PORTB, DBG);SET_BIT(PORTB, DBG);

#endif //_DENDRITE_H_