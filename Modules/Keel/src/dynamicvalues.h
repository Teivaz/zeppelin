#ifndef _DYNAMICVALUES_H_INCLUDED_
#define _DYNAMICVALUES_H_INCLUDED_

#include "protocol.h"

void initDv();

uint8_t readDv(PZ_Keel_DV);
void writeDv(PZ_Keel_DV, void const* value);
void resetDv(PZ_Keel_DV);
void resetAllDv();

void setBat0(uint8_t value);
uint8_t getBat0();
void setBat1(uint8_t value);
uint8_t getBat1();
void setTemp0(int8_t value);
int8_t getTemp0();

#endif // _DYNAMICVALUES_H_INCLUDED_
