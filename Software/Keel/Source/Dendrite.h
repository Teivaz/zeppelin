#ifndef DENDRITE_H_
#define DENDRITE_H_

// Call this once after initializations is done
void DendriteInit();

char DendriteReadReg(char reg);
void DendriteWriteReg(char reg, char data);
void DendriteWrite(char* data, char size);

void DendriteInterrupt();
void DendriteToggle(char front);

void DendriteStart();
void DendriteEnd();

void DendriteRead();
void DendriteFinishReading();
void DendriteInitRx();
void FlushRx();
void FlushTx();
char DendreiteReadByte();
void DendriteSetRegBit(char reg, char bit);
void DendriteClearRegBit(char reg, char bit);
#define PDBG		CLEAR_BIT(PORTB, DBG);SET_BIT(PORTB, DBG);

#endif //DENDRITE_H_