#include "program.h"
#include "stm32l0xx_hal.h"
#include "printf.h"

uint8_t s_on = 1;

void onTimer() {
	if (s_on) {
		printf("Timer triggered\r\n");
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	}
}

void toggleTimer() {
	s_on = !s_on;
	printf("Timer is %b\r\n", s_on);
}
