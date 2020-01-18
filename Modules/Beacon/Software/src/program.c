#include "program.h"
#include "stm32l0xx_hal.h"

uint8_t s_on = 1;

void onTimer() {
	if (s_on) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	}
}

void toggleTimer() {
	s_on = !s_on;
}
