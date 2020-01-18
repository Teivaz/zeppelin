#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"

RTC_HandleTypeDef s_rtc;

void Error_Handler() {
	*((char*)0) = 0U;
}
static void RTC_Init();
static void Clock_Init();
static void GPIO_Init();

int main(void) {
#ifdef _DEBUG
	volatile uint8_t stop = 0;
	while (stop);
#endif

	setup();
	while(1) {}
}

void setup() {
	HAL_Init();
	Clock_Init();
	RTC_Init();
	GPIO_Init();
}

void Clock_Init() {
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
	rccClk.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
															|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	rccClk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	rccClk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	rccClk.APB1CLKDivider = RCC_HCLK_DIV1;
	rccClk.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&rccClk, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}

	RCC_PeriphCLKInitTypeDef periphClck = {0};
	periphClck.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	periphClck.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&periphClck) != HAL_OK) {
		Error_Handler();
	}
}

void RTC_Init() {
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

void GPIO_Init() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin : PA1 */
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
}

void RTC_IRQHandler() {
	onTimer();
	HAL_RTC_AlarmIRQHandler(&s_rtc);
}

void SysTick_Handler(void) {
	HAL_IncTick();
}
