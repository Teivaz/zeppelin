#include "configurablevalues.h"
#include "stm32l0xx_hal.h"

__attribute__ ((section(".eeprom")))
struct {
	union {
		uint8_t _group0[0x10];
		uint8_t address; // address
	};
} s_configurableValues = {
	.address = 0x10,
};

static struct {
	uint8_t address;
} s_cv;

void resetCv(PZ_Keel_CV index) {
	switch (index) {
		case PZ_Keel_CV_address: setAddress(0x10);
		default: return;
	}
}

void resetAllCv() {
	// Do not reset address
}

void initCv() {
	s_cv.address = s_configurableValues.address;
}

uint8_t readCv(PZ_Keel_CV index) {
	switch (index) {
		case PZ_Keel_CV_address: return *(uint8_t*)&s_cv.address;
		default: return 0;
	}
}

static void* _writeCv(uint8_t offset, void const* data) {
	uint8_t* p = (uint8_t*)&s_configurableValues;
	p += (uint8_t)offset;
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)p, *(uint8_t*)data);
	HAL_FLASHEx_DATAEEPROM_Lock();
	return p;
}

void writeCv(PZ_Keel_CV index, void const* value) {
	switch (index) {
		case PZ_Keel_CV_address: return setAddress(*(uint8_t*)value);
		default: return;
	}
}

void setAddress(uint8_t value) {
	if (value != s_cv.address) {
		s_cv.address = *(uint8_t*)_writeCv(PZ_Keel_CV_address, &value);
	}
}
uint8_t getAddress() {
	return s_cv.address;
}
