#include "motor.h"
#include "main.h"
#include "dynamicvalues.h"
#include "configurablevalues.h"
#include "stm32l011xx.h"

// The Feather version 1.0 schematic does not take advantage of the hardware
// pwm channels. Thus the ports are triggered in software.
// The HAL takes t0o much time to wrap all the calls so this file does
// not use it for performance reasons.

static const uint16_t s_maxMotorTimerValue = 500; // Corresponds to 500 us

static void _setMotor(int8_t value);
static int32_t motorToTimerValue(int8_t value);

void motorInit() {
	_setMotor(getMotor());

	TIM2->ARR = s_maxMotorTimerValue;
	// Enable interrupts for compare 1, compare 2, and update
	TIM2->DIER |= TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;
}

static int32_t motorToTimerValue(int8_t value) {
	if (getMotorCalMode() == 0) {
		if (value < 0) {
			// value is in range -128 ... -1
			const int32_t range = -value; // 128 ... 1

			const int32_t motorRange = (255 - (128 + getMotorCal1())) * s_maxMotorTimerValue / 255;
			
			const int32_t motorValue = range * motorRange / 128;
			return -motorValue;
		}
		else if (value > 0) {
			// value is in range 1 ... 127
			const int32_t range = value; // 1 ... 127

			const int32_t motorRange = (128 + getMotorCal2()) * s_maxMotorTimerValue / 255;

			const int32_t motorValue = range * motorRange / 127;
			//const int32_t minimal = getMotorCal2();
			return motorValue;
		}
		else {
			return 0;
		}
	}
	else if (getMotorCalMode() == 1) {
		// value is in range -128 ... 127
		const int32_t range = 255 - (128 + value); // 255 ... 0
		const int32_t motorValue = range * s_maxMotorTimerValue / 255;
		return -motorValue;
	}
	else if (getMotorCalMode() == 2) {
		// value is in range -128 ... 127
		const int32_t range = 128 + value; // 0 ... 255
		const int32_t motorValue = range * s_maxMotorTimerValue / 255;
		return motorValue;
	}
	return 0;
}

static void _setMotor(int8_t value) {
	const int32_t tv = motorToTimerValue(value);
	if (tv > 0) {
		TIM2->CCR1 = (uint16_t)tv;
		TIM2->CCR2 = 0;
	}
	else if (tv < 0) {
		TIM2->CCR1 = 0;
		TIM2->CCR2 = (uint16_t)(-tv);
	}
	else {
		TIM2->CCR1 = 0;
		TIM2->CCR2 = 0;
	}
}

void DvOnMotorChanged(int8_t value) {
	_setMotor(value);
}

void DvOnMotorCalModeChanged(uint8_t value) {
	switch (value) {
		case 0: // Normal mode
			setMotor(getMotorDef());
			break;
		case 1: // Calibrating minimal value
			setMotor(getMotorCal1());
			break;
		case 2: // Calibrating maximal value
			setMotor(getMotorCal2());
			break;
		default:
			break;
	}
	_setMotor(getMotor());
}

void CvOnMotorCal1Changed(int8_t value) {
	(void)value;
	_setMotor(getMotor());
}

void CvOnMotorCal2Changed(int8_t value) {
	(void)value;
	_setMotor(getMotor());
}

void TIM2_IRQHandler() {
	// Compare 1 match
	if (TIM2->SR & TIM_SR_CC1IF) {
		TIM2->SR = ~TIM_SR_CC1IF; // Clear interrupt flag
		if (TIM2->CCR1 != TIM2->ARR) {
			GPIOA->BRR = GPIO_PIN_3;
		}
	}
	// Compare 2 match
	if (TIM2->SR & TIM_SR_CC2IF) {
		TIM2->SR = ~TIM_SR_CC2IF; // Clear interrupt flag
		if (TIM2->CCR2 != TIM2->ARR) {
			GPIOA->BRR = GPIO_PIN_4;
		}
	}
	// Timer update
	if (TIM2->SR & TIM_SR_UIF) {
		TIM2->SR = ~TIM_SR_UIF; // Clear interrupt flag
		GPIOA->BSRR = ((TIM2->CCR1 != 0) * GPIO_PIN_3) | ((TIM2->CCR2 != 0) * GPIO_PIN_4);
	}
}
