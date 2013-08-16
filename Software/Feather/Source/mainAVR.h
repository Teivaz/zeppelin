#ifndef MAINAVR_H_
#define MAINAVR_H_

#include "types.h"

#define PRIMARY_LETTER 'Z'
#define SECONDARY_LETTER '1'

int main();
void Init();
void SetMotorSpeedSigned(signed char speed);
void SetMotorSpeedUnsigned(unsigned char speed);
void SetServoPosition(unsigned char position);
void ReadSpi();

#endif /* MAINAVR_H_ */