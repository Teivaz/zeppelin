#ifndef __NRF24_HW_CONF_H
#define __NRF24_HW_CONF_H

#include "stm32l0xx_hal.h"

// Hardware abstraction layer for NRF24L01+ transceiver (hardware depended functions)
// GPIO pins definition
// GPIO pins initialization and control functions
// SPI transmit functions

// SPI port peripheral
#define NRF24_SPI_PORT             SPI1

// CE (chip enable) pin (PA4)
#define NRF24_CE_PORT              GPIOA
#define NRF24_CE_PIN               GPIO_PIN_4

// CSN (chip select negative) pin (PA3)
#define NRF24_CSN_PORT             GPIOA
#define NRF24_CSN_PIN              GPIO_PIN_3

// IRQ pin (PA2)
#define NRF24_IRQ_PORT             GPIOA
#define NRF24_IRQ_PIN              GPIO_PIN_2


#endif // __NRF24_HW_CONF_H
