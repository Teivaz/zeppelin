#include "dynamicvalues.h"
#include "configurablevalues.h"

static struct {
	int8_t motor;
	uint8_t motorCalMode;
	uint8_t servo;
	uint8_t servoCalMode;
} s_dv;

void resetDv(PZ_Feather_DV index) {
	switch (index) {
		case PZ_Feather_DV_motor: return setMotor(getMotorDef());
		case PZ_Feather_DV_motorCalMode: return setMotorCalMode(0);
		case PZ_Feather_DV_servo: return setServo(getServoDef());
		case PZ_Feather_DV_servoCalMode: return setServoCalMode(0);
	}
}

void resetAllDv() {
	setMotor(getMotorDef());
	setMotorCalMode(0);
	setServo(getServoDef());
	setServoCalMode(0);
}

void setMotor(int8_t value) {
	s_dv.motor = value;
}
int8_t getMotor() {
	return s_dv.motor;
}

void setMotorCalMode(uint8_t value) {
	s_dv.motorCalMode = value;
}
uint8_t getMotorCalMode() {
	return s_dv.motorCalMode;
}

void setServo(uint8_t value) {
	s_dv.servo = value;
}
uint8_t getServo() {
	return s_dv.servo;
}

void setServoCalMode(uint8_t value) {
	s_dv.servoCalMode = value;
}
uint8_t getServoCalMode() {
	return s_dv.servoCalMode;
}
