#include "servo.h"
#include "main.h"
#include "stm32l0xx_hal.h"
#include "dynamicvalues.h"
#include "configurablevalues.h"

static void _setServo(uint8_t value);
static uint16_t servoToTimerValue(uint8_t value);

static const uint16_t s_minServoTimerValue = 258; // Corresponds to 1ms
static const uint16_t s_maxServoTimerValue = 516; // Corresponds to 2ms

void servoInit() {
	_setServo(getServo());
	HAL_TIM_PWM_Start(GetTim21(), TIM_CHANNEL_1);
}

static uint16_t servoToTimerValue(uint8_t value) {
	if (getServoCalMode() == 0) {
		const uint32_t minimal = s_minServoTimerValue + getServoCal1() - 128;
		const uint32_t maximal = s_maxServoTimerValue + getServoCal2() - 128;
		const uint32_t range = maximal - minimal;
		const uint32_t normalized = (uint32_t)value * range / 0xff;
		return minimal + normalized;
	}
	else if (getServoCalMode() == 1) {
		return s_minServoTimerValue + value - 128;
	}
	else if (getServoCalMode() == 2) {
		return s_maxServoTimerValue + value - 128;
	}
	return s_minServoTimerValue;
}

static void _setServo(uint8_t value) {
	TIM21->CCR1 = servoToTimerValue(value);
}

void DvOnServoChanged(uint8_t value) {
	_setServo(value);
}
void CvOnServoCal1Changed(uint8_t value) {
	(void)value;
	_setServo(getServo());
}
void CvOnServoCal2Changed(uint8_t value) {
	(void)value;
	_setServo(getServo());
}
void DvOnServoCalModeChanged(uint8_t value) {
	switch (value) {
		case 0: // Normal mode
			setServo(getServoDef());
			break;
		case 1: // Calibrating minimal value
			setServo(getServoCal1());
			break;
		case 2: // Calibrating maximal value
			setServo(getServoCal2());
			break;
		default:
			break;
	}
	_setServo(getServo());
}
