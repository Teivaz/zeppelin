#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"
#include "configurablevalues.h"
#include "dynamicvalues.h"

RTC_HandleTypeDef s_rtc;
SPI_HandleTypeDef s_spi1;
CRC_HandleTypeDef s_crc;
I2C_HandleTypeDef s_i2c1;
ADC_HandleTypeDef s_adc;
DMA_HandleTypeDef s_dma_adc;

CRC_HandleTypeDef* GetCrc() { return &s_crc; }
SPI_HandleTypeDef* GetSpi() { return &s_spi1; }
I2C_HandleTypeDef* GetI2c() { return &s_i2c1; }
ADC_HandleTypeDef* GetAdc() { return &s_adc; }

void Error_Handler() {
	void __blocking_handler();
	__blocking_handler();
}
static void DMA_Init();
static void RTC_Init();
static void Clock_Init();
static void GPIO_Init();
static void SPI1_Init();
static void CRC_Init();
static void I2C1_Init();
static void ADC_Init();

int main(void) {
#ifdef _DEBUG
	volatile uint8_t stop = 0;
	while (stop);
#endif

	HAL_Init();
	Clock_Init();
	DMA_Init();
	RTC_Init();
	GPIO_Init();
	CRC_Init();
	ADC_Init();

	initCv();
	initDv();

	SPI1_Init();
	I2C1_Init();

	setup();

	while(1) {
		poll();
	}
}

static void Clock_Init() {
	// Configure the main internal regulator output voltage
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	// Initializes the CPU, AHB and APB busses clocks 
	RCC_OscInitTypeDef rccOsc = {0};
	rccOsc.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
	rccOsc.HSIState = RCC_HSI_ON;
	rccOsc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	rccOsc.LSIState = RCC_LSI_ON;
	rccOsc.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&rccOsc) != HAL_OK) {
		Error_Handler();
	}

	// Initializes the CPU, AHB and APB busses clocks
	RCC_ClkInitTypeDef rccClk = {0};
	rccClk.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	rccClk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	rccClk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	rccClk.APB1CLKDivider = RCC_HCLK_DIV1;
	rccClk.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&rccClk, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}

	RCC_PeriphCLKInitTypeDef periphClck = {0};
	periphClck.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RTC;
	periphClck.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	periphClck.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	periphClck.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&periphClck) != HAL_OK) {
		Error_Handler();
	}
}

static void RTC_Init() {
	// Initialize RTC Only
	s_rtc.Instance = RTC;
	s_rtc.Init.HourFormat = RTC_HOURFORMAT_24;
	s_rtc.Init.AsynchPrediv = 127;
	s_rtc.Init.SynchPrediv = 255;
	s_rtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	s_rtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	s_rtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	s_rtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&s_rtc) != HAL_OK) {
		Error_Handler();
	}

	// Initialize RTC and set the Time and Date
	RTC_TimeTypeDef rtcTime = {0};
	rtcTime.Hours = 0;
	rtcTime.Minutes = 0;
	rtcTime.Seconds = 0;
	rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	rtcTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&s_rtc, &rtcTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	RTC_DateTypeDef rtcDate = {0};
	rtcDate.WeekDay = RTC_WEEKDAY_MONDAY;
	rtcDate.Month = RTC_MONTH_JANUARY;
	rtcDate.Date = 1;
	rtcDate.Year = 0;
	if (HAL_RTC_SetDate(&s_rtc, &rtcDate, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	// Enable the Alarm A
	RTC_AlarmTypeDef rtcAlarm = {0};
	rtcAlarm.AlarmTime.Hours = 0;
	rtcAlarm.AlarmTime.Minutes = 0;
	rtcAlarm.AlarmTime.Seconds = 0;
	rtcAlarm.AlarmTime.SubSeconds = 0;
	rtcAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	rtcAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	rtcAlarm.AlarmMask = RTC_ALARMMASK_ALL;
	rtcAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	rtcAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	rtcAlarm.AlarmDateWeekDay = 1;
	rtcAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&s_rtc, &rtcAlarm, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
}

static void GPIO_Init() {
	GPIO_InitTypeDef port = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin : PA1 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	port.Pin = GPIO_PIN_1;
	port.Mode = GPIO_MODE_OUTPUT_PP;
	port.Pull = GPIO_NOPULL;
	port.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &port);	

	/*Configure GPIO pin : PB1 */
	port.Pin = GPIO_PIN_1;
	port.Mode = GPIO_MODE_INPUT;
	port.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &port);
}

static void SPI1_Init(void) {
	s_spi1.Instance = SPI1;
	s_spi1.Init.Mode = SPI_MODE_MASTER;
	s_spi1.Init.Direction = SPI_DIRECTION_2LINES;
	s_spi1.Init.DataSize = SPI_DATASIZE_8BIT;
	s_spi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	s_spi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	s_spi1.Init.NSS = SPI_NSS_SOFT;
	s_spi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	s_spi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	s_spi1.Init.TIMode = SPI_TIMODE_DISABLE;
	s_spi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	s_spi1.Init.CRCPolynomial = 7;
	if (HAL_SPI_Init(&s_spi1) != HAL_OK) {
		Error_Handler();
	}
}

static void CRC_Init() {
	s_crc.Instance = CRC;
	s_crc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
	s_crc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
	s_crc.Init.GeneratingPolynomial = 7;
	s_crc.Init.CRCLength = CRC_POLYLENGTH_8B;
	s_crc.Init.InitValue = 0;
	s_crc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
	s_crc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
	s_crc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
	if (HAL_CRC_Init(&s_crc) != HAL_OK) {
		Error_Handler();
	}
}

static void I2C1_Init() {
	s_i2c1.Instance = I2C1;
	s_i2c1.Init.Timing = 0x00303D5B;
	s_i2c1.Init.OwnAddress1 = 0;
	s_i2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	s_i2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	s_i2c1.Init.OwnAddress2 = 0;
	s_i2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	s_i2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	s_i2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&s_i2c1) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_I2CEx_ConfigAnalogFilter(&s_i2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_I2CEx_ConfigDigitalFilter(&s_i2c1, 0) != HAL_OK) {
		Error_Handler();
	}
}

static void ADC_Init() {
	// For HSI 16MHz
	// F_adc = 16/4 = 4MHz
	// 79.5 cycles will make 19.875us sample time - should be enough for any channel
	s_adc.Instance = ADC1;
	s_adc.Init.OversamplingMode = ENABLE;
	s_adc.Init.Oversample.Ratio = ADC_OVERSAMPLING_RATIO_16;
	s_adc.Init.Oversample.RightBitShift = ADC_RIGHTBITSHIFT_4;
	s_adc.Init.Oversample.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	s_adc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	s_adc.Init.Resolution = ADC_RESOLUTION_12B;
	s_adc.Init.SamplingTime = ADC_SAMPLETIME_79CYCLES_5;
	s_adc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	s_adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	s_adc.Init.ContinuousConvMode = ENABLE;
	s_adc.Init.DiscontinuousConvMode = DISABLE;
	s_adc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	s_adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	s_adc.Init.DMAContinuousRequests = ENABLE;
	s_adc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	s_adc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	s_adc.Init.LowPowerAutoWait = DISABLE;
	s_adc.Init.LowPowerFrequencyMode = DISABLE;
	s_adc.Init.LowPowerAutoPowerOff = DISABLE;
	if (HAL_ADC_Init(&s_adc) != HAL_OK) {
		Error_Handler();
	}
  ADC_ChannelConfTypeDef config = {0};
	config.Rank = ADC_RANK_CHANNEL_NUMBER;
	config.Channel = ADC_CHANNEL_0;
	if (HAL_ADC_ConfigChannel(&s_adc, &config) != HAL_OK) {
		Error_Handler();
	}
	config.Channel = ADC_CHANNEL_VREFINT; // Sample time at least 5us
	if (HAL_ADC_ConfigChannel(&s_adc, &config) != HAL_OK) {
		Error_Handler();
	}
	config.Channel = ADC_CHANNEL_TEMPSENSOR; // Sample time at least 10us
	if (HAL_ADC_ConfigChannel(&s_adc, &config) != HAL_OK) {
		Error_Handler();
	}
}

void DMA_Init() {
	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

uint8_t PZ_crc(uint8_t const* data, uint8_t size) {
	return HAL_CRC_Calculate(GetCrc(), (uint32_t*)data, size);
}

void SysTick_Handler() {
	HAL_IncTick();
}

void RTC_IRQHandler() {
	HAL_RTC_AlarmIRQHandler(&s_rtc);
	onTimer();
}

void I2C1_IRQHandler() {
	if (s_i2c1.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
		HAL_I2C_ER_IRQHandler(&s_i2c1);
	} else {
		HAL_I2C_EV_IRQHandler(&s_i2c1);
	}
}

void ADC1_COMP_IRQHandler() {
	HAL_ADC_IRQHandler(&s_adc);
}

void DMA1_Channel1_IRQHandler() {
	HAL_DMA_IRQHandler(&s_dma_adc);
}
