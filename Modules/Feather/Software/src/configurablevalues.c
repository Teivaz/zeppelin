#include "configurablevalues.h"
#include "main.h"

__attribute__ ((section(".eeprom")))
struct {
	union {
		uint8_t _group0[0x10];
		uint8_t address; // address
	};
	union {
		uint8_t _group1[0x10];
		struct {
			int8_t def; // default value for DC motor on startup
			int8_t cal1; // calibrating minimal value for DC motor
			int8_t cal2; // calibrating maximal value for DC motor
			int8_t cal0; // calibrating zero value for DC motor
		} motor;
	};
	union {
		uint8_t _group2[0x10];
		struct {
			uint8_t def; // default value for the servo on startup
			uint8_t cal1; // calibrating minimal value for the servo
			uint8_t cal2; // calibrating maximal value for the servo
		} servo;
	};
} s_configurableValues = {
	.address = 0x10,
	.motor = {
		.def = 0,
		.cal1 = -127,
		.cal2 = 127,
		.cal0 = 0,
	},
	.servo = {
		.def = 0,
		.cal1 = 0,
		.cal2 = 255,
	}
};

static struct {
	uint8_t address;
	int8_t motorDef;
	int8_t motorCal1;
	int8_t motorCal2;
	int8_t motorCal0;
	uint8_t servoDef;
	uint8_t servoCal1;
	uint8_t servoCal2;
} s_cv;

void initCv() {
	s_cv.address = s_configurableValues.address;
	s_cv.motorDef = s_configurableValues.motor.def;
	s_cv.motorCal1 = s_configurableValues.motor.cal1;
	s_cv.motorCal2 = s_configurableValues.motor.cal2;
	s_cv.motorCal0 = s_configurableValues.motor.cal0;
	s_cv.servoDef = s_configurableValues.servo.def;
	s_cv.servoCal1 = s_configurableValues.servo.cal1;
	s_cv.servoCal2 = s_configurableValues.servo.cal2;
}

uint8_t readCv(PZ_Feather_CV index) {
	switch (index) {
		case PZ_Feather_CV_addres: return *(uint8_t*)&s_cv.address;
		case PZ_Feather_CV_motorDef: return *(uint8_t*)&s_cv.motorDef;
		case PZ_Feather_CV_motorCal1: return *(uint8_t*)&s_cv.motorCal1;
		case PZ_Feather_CV_motorCal2: return *(uint8_t*)&s_cv.motorCal2;
		case PZ_Feather_CV_motorCal0: return *(uint8_t*)&s_cv.motorCal0;
		case PZ_Feather_CV_servoDef: return *(uint8_t*)&s_cv.servoDef;
		case PZ_Feather_CV_servoCal1: return *(uint8_t*)&s_cv.servoCal1;
		case PZ_Feather_CV_servoCal2: return *(uint8_t*)&s_cv.servoCal2;
		default: return 0;
	}
}

static void* _writeCv(uint8_t offset, void* data) {
	uint8_t* p = (uint8_t*)&s_configurableValues;
	p += (uint8_t)offset;
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)p, *(uint8_t*)data);
	HAL_FLASHEx_DATAEEPROM_Lock();
	return p;
}

void writeCv(PZ_Feather_CV index, void* value) {
	switch (index) {
		case PZ_Feather_CV_addres: return setAddress(*(uint8_t*)value);
		case PZ_Feather_CV_motorDef: return setMotorDef(*(int8_t*)value);
		case PZ_Feather_CV_motorCal1: return setMotorCal1(*(int8_t*)value);
		case PZ_Feather_CV_motorCal2: return setMotorCal2(*(int8_t*)value);
		case PZ_Feather_CV_motorCal0: return setMotorCal0(*(int8_t*)value);
		case PZ_Feather_CV_servoDef: return setServoDef(*(uint8_t*)value);
		case PZ_Feather_CV_servoCal1: return setServoCal1(*(uint8_t*)value);
		case PZ_Feather_CV_servoCal2: return setServoCal2(*(uint8_t*)value);
	}
}

void setAddress(uint8_t value) {
	if (value != s_cv.address) {
		s_cv.address = *(uint8_t*)_writeCv(PZ_Feather_CV_addres, &value);

		// Set I2C address here and reinit
		HAL_I2C_DeInit(GetI2c());
		GetI2c()->Init.OwnAddress1 = s_cv.address;
		HAL_I2C_Init(GetI2c());
	}
}
uint8_t getAddress() {
	return s_cv.address;
}

void setMotorDef(int8_t value) {
	if (value != s_cv.motorDef) {
		s_cv.motorDef = *(int8_t*)_writeCv(PZ_Feather_CV_motorDef, &value);
	}
}
int8_t getMotorDef() {
	return s_cv.motorDef;
}

void setMotorCal1(int8_t value) {
	if (value != s_cv.motorCal1) {
		s_cv.motorCal1 = *(int8_t*)_writeCv(PZ_Feather_CV_motorCal1, &value);
	}
}
int8_t getMotorCal1() {
	return s_cv.motorCal1;
}

void setMotorCal2(int8_t value) {
	if (value != s_cv.motorCal2) {
		s_cv.motorCal2 = *(int8_t*)_writeCv(PZ_Feather_CV_motorCal2, &value);
	}
}
int8_t getMotorCal2() {
	return s_cv.motorCal2;
}

void setMotorCal0(int8_t value) {
	if (value != s_cv.motorCal0) {
		s_cv.motorCal0 = *(int8_t*)_writeCv(PZ_Feather_CV_motorCal0, &value);
	}
}
int8_t getMotorCal0() {
	return s_cv.motorCal0;
}

void setServoDef(uint8_t value) {
	if (value != s_cv.servoDef) {
		s_cv.servoDef = *(uint8_t*)_writeCv(PZ_Feather_CV_servoDef, &value);
	}
}
uint8_t getServoDef() {
	return s_cv.servoDef;
}

void setServoCal1(uint8_t value) {
	if (value != s_cv.servoCal1) {
		s_cv.servoCal1 = *(uint8_t*)_writeCv(PZ_Feather_CV_servoCal1, &value);
	}
}
uint8_t getServoCal1() {
	return s_cv.servoCal1;
}

void setServoCal2(uint8_t value) {
	if (value != s_cv.servoCal2) {
		s_cv.servoCal2 = *(uint8_t*)_writeCv(PZ_Feather_CV_servoCal2, &value);
	}
}
uint8_t getServoCal2() {
	return s_cv.servoCal2;
}
