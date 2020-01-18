#include "program.h"
#include "stm32l0xx_hal.h"

void onTimer() {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
}
