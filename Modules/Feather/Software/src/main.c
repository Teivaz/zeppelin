#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"
#include "configurablevalues.h"
#include "dynamicvalues.h"
#include "motor.h"
#include "servo.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef*);

RTC_HandleTypeDef s_rtc;
CRC_HandleTypeDef s_crc;
I2C_HandleTypeDef s_i2c1;
TIM_HandleTypeDef s_tim21;
TIM_HandleTypeDef s_tim2;

CRC_HandleTypeDef* GetCrc() { return &s_crc; }
I2C_HandleTypeDef* GetI2c() { return &s_i2c1; }
TIM_HandleTypeDef* GetTim21() { return &s_tim21; }
TIM_HandleTypeDef* GetTim2() { return &s_tim2; }

void Error_Handler() {
	void __blocking_handler();
	__blocking_handler();
}
static void RTC_Init();
static void Clock_Init();
static void GPIO_Init();
static void CRC_Init();
static void I2C1_Init();
static void TIM21_Init();
static void TIM2_Init();

int main(void) {
#ifdef _DEBUG
	volatile uint8_t stop = 0;
	while (stop);
#endif

	HAL_Init();
	Clock_Init();
	RTC_Init();
	GPIO_Init();
	CRC_Init();
	TIM21_Init();
	TIM2_Init();
	
	initCv();
	initDv();

	I2C1_Init();

	HAL_TIM_MspPostInit(&s_tim21);
	HAL_TIM_MspPostInit(&s_tim2);
	motorInit();
	servoInit();

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

	/*Configure GPIO pin : PA1 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	port.Pin = GPIO_PIN_1;
	port.Mode = GPIO_MODE_OUTPUT_PP;
	port.Pull = GPIO_NOPULL;
	port.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &port);
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
	s_i2c1.Init.OwnAddress1 = getAddress();
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

static void TIM21_Init() {
	TIM_ClockConfigTypeDef clockSourceConfig = {0};
	TIM_MasterConfigTypeDef masterConfig = {0};
	TIM_OC_InitTypeDef configOc = {0};

	s_tim21.Instance = TIM21;
	s_tim21.Init.Prescaler = 61; // Internally adds 1
	s_tim21.Init.CounterMode = TIM_COUNTERMODE_UP;
	s_tim21.Init.Period = 5160;
	s_tim21.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	s_tim21.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&s_tim21) != HAL_OK) {
		Error_Handler();
	}
	clockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&s_tim21, &clockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&s_tim21) != HAL_OK) {
		Error_Handler();
	}
	masterConfig.MasterOutputTrigger = TIM_TRGO_OC1;
	masterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&s_tim21, &masterConfig) != HAL_OK) {
		Error_Handler();
	}
	configOc.OCMode = TIM_OCMODE_PWM1;
	configOc.Pulse = 258;
	configOc.OCPolarity = TIM_OCPOLARITY_HIGH;
	configOc.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&s_tim21, &configOc, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
}

static void TIM2_Init() {
	TIM_ClockConfigTypeDef clockSourceConfig = {0};
	TIM_MasterConfigTypeDef masterConfig = {0};
	TIM_OC_InitTypeDef configOC = {0};

	s_tim2.Instance = TIM2;
	s_tim2.Init.Prescaler = 15; // Internally adds 1
	s_tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	s_tim2.Init.Period = 2000;
	s_tim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	s_tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&s_tim2) != HAL_OK) {
		Error_Handler();
	}
	clockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&s_tim2, &clockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&s_tim2) != HAL_OK) {
		Error_Handler();
	}
	masterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	masterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&s_tim2, &masterConfig) != HAL_OK) {
		Error_Handler();
	}
	configOC.OCMode = TIM_OCMODE_PWM1;
	configOC.Pulse = 0;
	configOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	configOC.OCFastMode = TIM_OCFAST_ENABLE;
	if (HAL_TIM_PWM_ConfigChannel(&s_tim2, &configOC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&s_tim2, &configOC, TIM_CHANNEL_2) != HAL_OK) {
		Error_Handler();
	}
}

void SysTick_Handler() {
	HAL_IncTick();
}

void RTC_IRQHandler() {
	onTimer();
	HAL_RTC_AlarmIRQHandler(&s_rtc);
}

void EXTI0_1_IRQHandler() {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void I2C1_IRQHandler() {
	if (s_i2c1.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
		HAL_I2C_ER_IRQHandler(&s_i2c1);
	} else {
		HAL_I2C_EV_IRQHandler(&s_i2c1);
	}
}
