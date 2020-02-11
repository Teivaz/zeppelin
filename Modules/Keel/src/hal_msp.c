#include "stm32l0xx_hal.h"

extern DMA_HandleTypeDef s_dma_adc;

void Error_Handler();

// Initializes the Global MSP
void HAL_MspInit(void) {
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc) {
	__HAL_RCC_RTC_ENABLE();
	HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(RTC_IRQn);
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc) {
	__HAL_RCC_RTC_DISABLE();
	HAL_NVIC_DisableIRQ(RTC_IRQn);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
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

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi) {
	__HAL_RCC_SPI1_CLK_DISABLE();
	/**SPI1 GPIO Configuration    
	PA5     ------> SPI1_SCK
	PA6     ------> SPI1_MISO
	PA7     ------> SPI1_MOSI 
	*/
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
}

void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc) {
	__HAL_RCC_CRC_CLK_ENABLE();
}

void HAL_CRC_MspDeInit(CRC_HandleTypeDef* hcrc) {
	__HAL_RCC_CRC_CLK_DISABLE();
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**I2C1 GPIO Configuration    
	PA9  ------> I2C1_SCL
	PA10 ------> I2C1_SDA 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	__HAL_RCC_I2C1_CLK_ENABLE();
	HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C1_IRQn);
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c) {
	__HAL_RCC_I2C1_CLK_DISABLE();
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
	HAL_NVIC_DisableIRQ(I2C1_IRQn);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	// PA0-CK_IN ------> ADC_IN0
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	s_dma_adc.Instance = DMA1_Channel1;
	s_dma_adc.Init.Request = DMA_REQUEST_0;
	s_dma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
	s_dma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
	s_dma_adc.Init.MemInc = DMA_MINC_ENABLE;
	s_dma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	s_dma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	s_dma_adc.Init.Mode = DMA_CIRCULAR;
	s_dma_adc.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&s_dma_adc) != HAL_OK) {
		Error_Handler();
	}

	__HAL_LINKDMA(hadc, DMA_Handle, s_dma_adc);

	HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc) {
	__HAL_RCC_ADC1_CLK_DISABLE();
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
	HAL_DMA_DeInit(hadc->DMA_Handle);
	HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
}
