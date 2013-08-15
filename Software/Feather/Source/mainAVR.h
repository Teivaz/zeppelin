#ifndef MAINAVR_H_
#define MAINAVR_H_

#include "types.h"

int main();
void Init();
void SetMotorSpeed(signed char speed);
void SetServoPosition(unsigned char position);

// temporary for tests
void UpdateCounter(unsigned char counter);

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