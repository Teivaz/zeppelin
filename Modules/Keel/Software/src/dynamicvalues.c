#include "dynamicvalues.h"

static struct {
	uint8_t bat0;
	uint8_t bat1;
	uint8_t thermo;
} s_dv;

uint8_t readDv(PZ_Keel_DV index) {
	switch (index) {
		case PZ_Keel_DV_bat0: return *(uint8_t*)&s_dv.bat0;
		case PZ_Keel_DV_bat1: return *(uint8_t*)&s_dv.bat1;
		case PZ_Keel_DV_thermo: return *(uint8_t*)&s_dv.thermo;
		default: return 0;
	}
}

void writeDv(PZ_Keel_DV index, void const* value) {
	switch (index) {
		case PZ_Keel_DV_bat0: return setBat0(*(uint8_t*)value);
		case PZ_Keel_DV_bat1: return setBat1(*(uint8_t*)value);
		case PZ_Keel_DV_thermo: return setThermo(*(uint8_t*)value);
		default: return;
	}	
}

void resetDv(PZ_Keel_DV index) {
	switch (index) {
		case PZ_Keel_DV_bat0: return setBat0(0);
		case PZ_Keel_DV_bat1: return setBat1(0);
		case PZ_Keel_DV_thermo: return setThermo(0);
		default: return;
	}
}

void resetAllDv() {
	setBat0(0);
	setBat1(0);
	setThermo(0);
}

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

void setThermo(uint8_t value) {
	s_dv.thermo = value;
}
uint8_t getThermo() {
	return s_dv.thermo;
}
