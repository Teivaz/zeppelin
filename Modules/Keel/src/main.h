#ifndef _MAIN_H_INCLUDED_
#define _MAIN_H_INCLUDED_

#include "stm32l0xx_hal.h"

void SysTick_Handler();
void RTC_IRQHandler();

CRC_HandleTypeDef* GetCrc();
SPI_HandleTypeDef* GetSpi();
I2C_HandleTypeDef* GetI2c();
ADC_HandleTypeDef* GetAdc();

int main();

#endif // _MAIN_H_INCLUDED_
