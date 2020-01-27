#ifndef _DYNAMICVALUES_H_INCLUDED_
#define _DYNAMICVALUES_H_INCLUDED_

#include "protocol.h"

uint8_t readDv(PZ_Feather_DV);
void writeDv(PZ_Feather_DV, void* value);

void resetDv(PZ_Feather_DV);
void resetAllDv();

void setMotor(int8_t value);
void setMotorCalMode(uint8_t value);
void setServo(uint8_t value);
void setServoCalMode(uint8_t value);

int8_t getMotor();
uint8_t getMotorCalMode();
uint8_t getServo();
uint8_t getServoCalMode();

#endif // _DYNAMICVALUES_H_INCLUDED_
