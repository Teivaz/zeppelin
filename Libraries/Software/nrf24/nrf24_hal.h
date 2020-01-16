#pragma once

#include <stdint.h>

void NRF24_GPIO_Init();
void NRF24_SPI_Init();

void NRF24_CE_Low();
void NRF24_CE_High();

void NRF24_CSN_Low();
void NRF24_CSN_High();

void NRF24_Wait_10us();

uint8_t NRF24_LL_RW(uint8_t data);
