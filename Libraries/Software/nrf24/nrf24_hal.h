#ifndef _NRF24_HAL_H_INCLUDED_
#define _NRF24_HAL_H_INCLUDED_

#include <stdint.h>

void NRF24_GPIO_Init();
void NRF24_SPI_Init(void* spi);

void NRF24_CE_Low();
void NRF24_CE_High();

void NRF24_CSN_Low();
void NRF24_CSN_High();

uint8_t NRF24_LL_RW(uint8_t data);

#endif // _NRF24_HAL_H_INCLUDED_
