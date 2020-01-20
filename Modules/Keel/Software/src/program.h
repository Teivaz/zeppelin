#pragma once
#include "protocol.h"

void setup();
void onTimer();
void onExtIrq();
void sendPz(PZ_Package* p);
