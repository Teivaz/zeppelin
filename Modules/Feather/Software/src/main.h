#ifndef _MAIN_H_INCLUDED_
#define _MAIN_H_INCLUDED_

#include "stm32l0xx_hal.h"

void SysTick_Handler();
void RTC_IRQHandler();
void EXTI0_1_IRQHandler();
void I2C1_IRQHandler();

CRC_HandleTypeDef* GetCrc();
I2C_HandleTypeDef* GetI2c();

int main();

#endif // _MAIN_H_INCLUDED_
