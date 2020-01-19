#include "keel.h"
#include "main.h"
#include "printf.h"

__attribute__ ((section(".eeprom")))
struct {
	uint8_t address;
} s_configurableValues = {
	.address = 0x10,
};

static uint8_t s_address;

void initKeel() {
	s_address = s_configurableValues.address;
}

uint8_t getAddress() {
	return s_address;
}

void setAddress(uint8_t address) {
	s_configurableValues.address = address;
	s_address = address;
}

void processPackage(PZ_Package* p) {
	if (p->adr == getAddress()) {
		printf("it's me!\n\r");
	}
}
