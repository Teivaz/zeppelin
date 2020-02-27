#pragma once

#include "stm32wbxx_hal.h"

#define Led1Pin GPIO_PIN_5
#define Led1Port GPIOB
#define Led2Pin GPIO_PIN_0
#define Led2Port GPIOB
#define Led3Pin GPIO_PIN_1
#define Led3Port GPIOB

#define Bt1Pin GPIO_PIN_4
#define Bt1Port GPIOC
#define Bt1Irq EXTI4_IRQn
#define Bt2Pin GPIO_PIN_0
#define Bt2Port GPIOD
#define Bt3Pin GPIO_PIN_1
#define Bt3Port GPIOD

#define STLINK_RX_Pin GPIO_PIN_6
#define STLINK_RX_GPIO_Port GPIOB
#define STLINK_TX_Pin GPIO_PIN_7
#define STLINK_TX_GPIO_Port GPIOB

void ErrorHandler(void);
DMA_HandleTypeDef* getUsartDma();
RTC_HandleTypeDef* getRtc();
UART_HandleTypeDef* geUart1();
