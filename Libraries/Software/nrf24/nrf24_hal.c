#include "nrf24_hal.h"
#include "nrf24_hw_conf.h"

SPI_HandleTypeDef* s_spi = 0;

void NRF24_CE_Low() {
	HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_RESET);
}
void NRF24_CE_High() {
	HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_SET);
}	

void NRF24_CSN_Low() {
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET);
}
void NRF24_CSN_High() {
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);
}

// Configure the GPIO lines of the NRF24L01 transceiver
// note: IRQ pin must be configured separately
void NRF24_GPIO_Init() {

	GPIO_InitTypeDef port = {0};

	/*Configure GPIO pin : CSN */
	NRF24_CSN_High();
	port.Pin = NRF24_CSN_PIN;
	port.Mode = GPIO_MODE_OUTPUT_PP;
	port.Pull = GPIO_NOPULL;
	port.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(NRF24_CSN_PORT, &port);

	/*Configure GPIO pin : CE */
	NRF24_CE_Low();
	port.Pin = NRF24_CE_PIN;
	port.Mode = GPIO_MODE_OUTPUT_PP;
	port.Pull = GPIO_NOPULL;
	port.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(NRF24_CE_PORT, &port);

#if 0
	// Interrupt will go here
	/*Configure GPIO pin : PA3 */
	port.Pin = NRF24_IRQ_PIN;
	port.Mode = GPIO_MODE_IT_RISING;
	port.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(NRF24_IRQ_PORT, &port);
#endif
}

void NRF24_SPI_Init(void* spi) {
	s_spi = (SPI_HandleTypeDef*)spi;
}

// Low level SPI transmit/receive function (hardware depended)
// input:
//   data - value to transmit via SPI
// return: value received from SPI
uint8_t NRF24_LL_RW(uint8_t data) {
	 // Wait until TX buffer is empty
	HAL_SPI_StateTypeDef state = HAL_SPI_GetState(s_spi);
	while (state == HAL_SPI_STATE_BUSY_TX) {
		state = HAL_SPI_GetState(s_spi);
	}

	uint8_t result;
	HAL_SPI_TransmitReceive(s_spi, &data, &result, 1, HAL_MAX_DELAY);
	return result;
}
