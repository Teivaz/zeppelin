#pragma once
#include "stm32l0xx_hal.h"

void SysTick_Handler();
void RTC_IRQHandler();
void EXTI0_1_IRQHandler();

CRC_HandleTypeDef* GetCrc();
SPI_HandleTypeDef* GetSpi();

int main();
