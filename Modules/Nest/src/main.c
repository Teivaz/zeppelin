#include "main.h"
#include "wpanapp.h"
#include "app_common.h"
#include "stm32_seq.h"
#include <stdio.h>

static RTC_HandleTypeDef sRtc;
static UART_HandleTypeDef sUart1;
static DMA_HandleTypeDef sDmaUsart1Tx;
static PCD_HandleTypeDef sPcdUsbFs;

DMA_HandleTypeDef* getUsartDma() { return &sDmaUsart1Tx; }
RTC_HandleTypeDef* getRtc() { return &sRtc; }
UART_HandleTypeDef* geUart1() { return &sUart1; }

static void ConfigureSystemClock(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_RF_Init(void);
static void MX_RTC_Init(void);

int main(void) {
	HAL_Init();

	ConfigureSystemClock();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_USB_PCD_Init();
	MX_RF_Init();
	MX_RTC_Init();

	WpanAppInit();

	while(1) {
		UTIL_SEQ_Run(~0);
	}
}

void ConfigureSystemClock(void) {
	__HAL_RCC_PLL_PLLM_CONFIG(RCC_PLLM_DIV1);
	__HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_MSI);
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitTypeDef oscInit = {0};
	oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
	oscInit.HSEState = RCC_HSE_ON;
	oscInit.LSEState = RCC_LSE_ON;
	oscInit.HSIState = RCC_HSI_ON;
	oscInit.MSIState = RCC_MSI_ON;
	oscInit.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	oscInit.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
	oscInit.MSIClockRange = RCC_MSIRANGE_6;
	oscInit.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&oscInit) != HAL_OK) {
		ErrorHandler();
	}

	RCC_ClkInitTypeDef clockInit = {0};
	clockInit.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	clockInit.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clockInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clockInit.APB1CLKDivider = RCC_HCLK_DIV1;
	clockInit.APB2CLKDivider = RCC_HCLK_DIV1;
	clockInit.AHBCLK2Divider = RCC_SYSCLK_DIV1;
	clockInit.AHBCLK4Divider = RCC_SYSCLK_DIV1;
	if (HAL_RCC_ClockConfig(&clockInit, FLASH_LATENCY_1) != HAL_OK) {
		ErrorHandler();
	}

	RCC_PeriphCLKInitTypeDef periphClockInit = {0};
	periphClockInit.PeriphClockSelection = RCC_PERIPHCLK_SMPS|RCC_PERIPHCLK_RFWAKEUP|RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USB;
	periphClockInit.PLLSAI1.PLLN = 24;
	periphClockInit.PLLSAI1.PLLP = RCC_PLLP_DIV2;
	periphClockInit.PLLSAI1.PLLQ = RCC_PLLQ_DIV2;
	periphClockInit.PLLSAI1.PLLR = RCC_PLLR_DIV2;
	periphClockInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_USBCLK;
	periphClockInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	periphClockInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
	periphClockInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	periphClockInit.RFWakeUpClockSelection = RCC_RFWKPCLKSOURCE_LSE;
	periphClockInit.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSE;
	periphClockInit.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE0;

	if (HAL_RCCEx_PeriphCLKConfig(&periphClockInit) != HAL_OK) {
		ErrorHandler();
	}
	HAL_RCCEx_EnableMSIPLLMode();
}

static void MX_RF_Init(void) {
}

static void MX_RTC_Init(void) {
	sRtc.Instance = RTC;
	sRtc.Init.HourFormat = RTC_HOURFORMAT_24;
	sRtc.Init.AsynchPrediv = CFG_RTC_ASYNCH_PRESCALER;
	sRtc.Init.SynchPrediv = CFG_RTC_SYNCH_PRESCALER;
	if (HAL_RTC_Init(&sRtc) != HAL_OK) {
		ErrorHandler();
	}
}

static void MX_USART1_UART_Init(void) {
	sUart1.Instance = USART1;
	sUart1.Init.BaudRate = 115200;
	sUart1.Init.WordLength = UART_WORDLENGTH_8B;
	sUart1.Init.StopBits = UART_STOPBITS_1;
	sUart1.Init.Parity = UART_PARITY_NONE;
	sUart1.Init.Mode = UART_MODE_TX_RX;
	sUart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	sUart1.Init.OverSampling = UART_OVERSAMPLING_16;
	sUart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	sUart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	sUart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&sUart1) != HAL_OK) {
		ErrorHandler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&sUart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
		ErrorHandler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&sUart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
		ErrorHandler();
	}
	if (HAL_UARTEx_DisableFifoMode(&sUart1) != HAL_OK) {
		ErrorHandler();
	}
}

static void MX_USB_PCD_Init(void) {
	sPcdUsbFs.Instance = USB;
	sPcdUsbFs.Init.dev_endpoints = 8;
	sPcdUsbFs.Init.speed = PCD_SPEED_FULL;
	sPcdUsbFs.Init.phy_itface = PCD_PHY_EMBEDDED;
	sPcdUsbFs.Init.Sof_enable = DISABLE;
	sPcdUsbFs.Init.low_power_enable = DISABLE;
	sPcdUsbFs.Init.lpm_enable = DISABLE;
	sPcdUsbFs.Init.battery_charging_enable = DISABLE;
	if (HAL_PCD_Init(&sPcdUsbFs) != HAL_OK) {
		ErrorHandler();
	}
}

static void MX_DMA_Init(void) {
	__HAL_RCC_DMAMUX1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOB, Led1Pin | Led2Pin | Led3Pin, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = Bt1Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(Bt1Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Led2Pin | Led3Pin | Led1Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Bt2Pin | Bt3Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

void ErrorHandler(void) {
}
