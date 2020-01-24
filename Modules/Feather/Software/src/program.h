#pragma once
#include "protocol.h"

void setup();
void onTimer();
void onExtIrq();
void sendPz(PZ_Package* p);

uint8_t getBattery0();
uint8_t getBattery1();
uint8_t getThermometer0();
