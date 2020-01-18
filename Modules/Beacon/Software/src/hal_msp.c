#include "stm32l0xx_hal.h"

// Initializes the Global MSP
void HAL_MspInit(void) {
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
}

// RTC MSP Initialization
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc) {
	if(hrtc->Instance == RTC) {
		/* Peripheral clock enable */
		__HAL_RCC_RTC_ENABLE();
		/* RTC interrupt Init */
		HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(RTC_IRQn);
	}
}

// RTC MSP De-Initialization
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc) {
	if(hrtc->Instance == RTC) {
		/* Peripheral clock disable */
		__HAL_RCC_RTC_DISABLE();

		/* RTC interrupt DeInit */
		HAL_NVIC_DisableIRQ(RTC_IRQn);
	}
}
