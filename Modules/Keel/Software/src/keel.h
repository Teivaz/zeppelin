#pragma once
#include "protocol.h"

void processPackage(PZ_Package* package, uint8_t* raw, uint8_t size);

void initKeel();

uint8_t getAddress();
void setAddress(uint8_t);
