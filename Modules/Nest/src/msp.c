#include "main.h"
#include "otp.h"

void HAL_MspInit(void) {
	OTP_ID0_t* otp = (OTP_ID0_t*)OTP_Read(0);
	if (otp) {
		LL_RCC_HSE_SetCapacitorTuning(otp->hse_tuning);
	}
	__HAL_RCC_HSEM_CLK_ENABLE();
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc) {
	if (hrtc->Instance == RTC) {
		__HAL_RCC_RTC_ENABLE();
	}
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc) {
	if (hrtc->Instance == RTC) {
		__HAL_RCC_RTC_DISABLE();
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* uart) {
	if (uart->Instance == USART1) {
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		// PB6 -> USART1_TX
		// PB7 -> USART1_RX
		GPIO_InitTypeDef gpio = {0};
		gpio.Pin = STLINK_RX_Pin | STLINK_TX_Pin;
		gpio.Mode = GPIO_MODE_AF_PP;
		gpio.Pull = GPIO_NOPULL;
		gpio.Speed = GPIO_SPEED_FREQ_LOW;
		gpio.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOB, &gpio);
	
		DMA_HandleTypeDef* dma = getUsartDma();
		dma->Instance = DMA1_Channel1;
		dma->Init.Request = DMA_REQUEST_USART1_TX;
		dma->Init.Direction = DMA_MEMORY_TO_PERIPH;
		dma->Init.PeriphInc = DMA_PINC_DISABLE;
		dma->Init.MemInc = DMA_MINC_ENABLE;
		dma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		dma->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		dma->Init.Mode = DMA_NORMAL;
		dma->Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(dma) != HAL_OK) {
			ErrorHandler();
		}

		__HAL_LINKDMA(uart, hdmatx, *dma);

		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uart) {
	if (uart->Instance == USART1) {
		__HAL_RCC_USART1_CLK_DISABLE();
		// PB6 -> USART1_TX
		// PB7 -> USART1_RX
		HAL_GPIO_DeInit(GPIOB, STLINK_RX_Pin | STLINK_TX_Pin);
		HAL_DMA_DeInit(uart->hdmatx);	
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
}

void HAL_PCD_MspInit(PCD_HandleTypeDef* pcd) {
	if (pcd->Instance == USB) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
		// PA11 -> USB_DM
		// PA12 -> USB_DP
		GPIO_InitTypeDef gpio = {0};
		gpio.Pin = GPIO_PIN_11 | GPIO_PIN_12;
		gpio.Mode = GPIO_MODE_AF_PP;
		gpio.Pull = GPIO_NOPULL;
		gpio.Speed = GPIO_SPEED_FREQ_LOW;
		gpio.Alternate = GPIO_AF10_USB;
		HAL_GPIO_Init(GPIOA, &gpio);
		__HAL_RCC_USB_CLK_ENABLE();
	}
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* pcd) {
	if (pcd->Instance == USB) {
		__HAL_RCC_USB_CLK_DISABLE();
		// PA11 -> USB_DM
		// PA12 -> USB_DP
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);
	}
}



