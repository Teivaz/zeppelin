#include "dynamicvalues.h"

static struct {
	uint8_t bat0;
	uint8_t bat1;
	int8_t temp0;
} s_dv;

void initDv() {
	s_dv.bat0 = 0;
	s_dv.bat1 = 0;
	s_dv.temp0 = 0;
}

uint8_t readDv(PZ_Keel_DV index) {
	switch (index) {
		case PZ_Keel_DV_bat0: return *(uint8_t*)&s_dv.bat0;
		case PZ_Keel_DV_bat1: return *(uint8_t*)&s_dv.bat1;
		case PZ_Keel_DV_temp0: return *(int8_t*)&s_dv.temp0;
		default: return 0;
	}
}

void writeDv(PZ_Keel_DV index, void const* value) {
	(void)index;
	(void)value;
}

void resetDv(PZ_Keel_DV index) {
	(void)index;
}

void resetAllDv() {}

void setBat0(uint8_t value) {
	s_dv.bat0 = value;
}
uint8_t getBat0() {
	return s_dv.bat0;
}

void setBat1(uint8_t value) {
	s_dv.bat1 = value;
}
uint8_t getBat1() {
	return s_dv.bat1;
}

void setTemp0(int8_t value) {
	s_dv.temp0 = value;
}
int8_t getTemp0() {
	return s_dv.temp0;
}
