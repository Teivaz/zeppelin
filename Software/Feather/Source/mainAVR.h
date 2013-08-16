#ifndef MAINAVR_H_
#define MAINAVR_H_

#include "types.h"

int main();
void Init();
void SetMotorSpeedSigned(signed char speed);
void SetMotorSpeedUnsigned(unsigned char speed);
void SetServoPosition(unsigned char position);

// temporary for tests
void UpdateCounter(unsigned char counter);
char IsValidHeader(char header);
void ReadSpi();



// SPI
extern inline bool ReadSpiDataBit(void)
{
	return false;
}

extern inline void WriteSpiDataBit(bool bit)
{
	;
}

#endif /* MAINAVR_H_ */