#include "keel.h"
#include "main.h"
#include "printf.h"
#include "program.h"

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
	HAL_FLASHEx_DATAEEPROM_Unlock();
	s_configurableValues.address = address;
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)&s_configurableValues.address, address);
	HAL_FLASHEx_DATAEEPROM_Lock();
	s_address = s_configurableValues.address;
}

void processPackage(PZ_Package* p, uint8_t* raw, uint8_t size) {
	if (p->adr == getAddress()) {
		if (p->cmd == PZ_Cmd_Info) {
			PZ_Package r = PZ_composeRe2(p, PZ_KEEL_TYPE, PZ_VERSION);
			sendPz(&r);
		}
		else if (p->cmd == PZ_Cmd_Read_cv) {
			if (p->pld[0] == PZ_Keel_CV_addres) {
				PZ_Package r = PZ_composeRe2(p, PZ_Keel_CV_addres, getAddress());
				sendPz(&r);
			}
		}
		else if (p->cmd == PZ_Cmd_Write_cv) {
			if (p->pld[0] == PZ_Keel_CV_addres) {
				if (PZ_isAdrValid(p->pld[1])) {
					setAddress(p->pld[1]);
				}
			}
		}
		else if (p->cmd == PZ_Cmd_Read_dv) {
			if (p->pld[0] == PZ_Keel_DV_bat0) {
				PZ_Package r = PZ_composeRe2(p, PZ_Keel_DV_bat0, getBattery0());
				sendPz(&r);
			}
			else if (p->pld[0] == PZ_Keel_DV_bat1) {
				PZ_Package r = PZ_composeRe2(p, PZ_Keel_DV_bat1, getBattery1());
				sendPz(&r);
			}
			else if (p->pld[0] == PZ_Keel_DV_thermo) {
				PZ_Package r = PZ_composeRe2(p, PZ_Keel_DV_thermo, getThermometer0());
				sendPz(&r);
			}
		}
	}
	else {
		HAL_I2C_Master_Transmit(GetI2c(), p->adr, raw, size, HAL_MAX_DELAY);
	}
}
