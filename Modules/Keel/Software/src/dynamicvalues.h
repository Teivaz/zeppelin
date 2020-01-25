#pragma once
#include "protocol.h"

uint8_t readDv(PZ_Keel_DV);
void writeDv(PZ_Keel_DV, void* value);

void resetDv(PZ_Keel_DV);
void resetAllDv();

void setBat0(uint8_t value);
void setBat1(uint8_t value);
void setThermo(uint8_t value);

uint8_t getBat0();
uint8_t getBat1();
uint8_t getThermo();
