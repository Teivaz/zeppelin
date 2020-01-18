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

void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(huart->Instance == USART2) {
		__HAL_RCC_USART2_CLK_ENABLE();
	
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF0_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart) {
	if(huart->Instance == USART2) {
		__HAL_RCC_USART2_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_2);
	}
}


void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(hspi->Instance == SPI1) {
		__HAL_RCC_SPI1_CLK_ENABLE();
	
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**SPI1 GPIO Configuration    
		PA5     ------> SPI1_SCK
		PA6     ------> SPI1_MISO
		PA7     ------> SPI1_MOSI 
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi) {
	if(hspi->Instance == SPI1) {
		__HAL_RCC_SPI1_CLK_DISABLE();
		/**SPI1 GPIO Configuration    
		PA5     ------> SPI1_SCK
		PA6     ------> SPI1_MISO
		PA7     ------> SPI1_MOSI 
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	}
}

void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc) {
  if(hcrc->Instance == CRC) {
    __HAL_RCC_CRC_CLK_ENABLE();
  }
}

void HAL_CRC_MspDeInit(CRC_HandleTypeDef* hcrc) {
  if(hcrc->Instance == CRC) {
    __HAL_RCC_CRC_CLK_DISABLE();
  }
}
