#include "nrf24_hal.h"
#include "nrf24_hw_conf.h"

static SPI_HandleTypeDef s_spi = {0};

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

void NRF24_SPI_Init() {
	s_spi.Instance = NRF24_SPI_PORT;
	s_spi.Init.Mode = SPI_MODE_MASTER;
	s_spi.Init.Direction = SPI_DIRECTION_2LINES;
	s_spi.Init.DataSize = SPI_DATASIZE_8BIT;
	s_spi.Init.CLKPolarity = SPI_POLARITY_LOW;
	s_spi.Init.CLKPhase = SPI_PHASE_1EDGE;
	s_spi.Init.NSS = SPI_NSS_SOFT;
	s_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	s_spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	s_spi.Init.TIMode = SPI_TIMODE_DISABLE;
	s_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	s_spi.Init.CRCPolynomial = 7;
	if (HAL_SPI_Init(&s_spi) != HAL_OK)
	{
		//Error_Handler();
	}
}

// Time to allow the transmission
void NRF24_Wait_10us() {
	// Wait for 160 cycles at 16 MHz = 10us.
	for(int8_t i = 160; i > 0; --i)
	{
		__asm__ __volatile__ ("nop"::);
	}
}

// Low level SPI transmit/receive function (hardware depended)
// input:
//   data - value to transmit via SPI
// return: value received from SPI
uint8_t NRF24_LL_RW(uint8_t data) {
	 // Wait until TX buffer is empty
	HAL_SPI_StateTypeDef state = HAL_SPI_GetState(&s_spi);
	while (state == HAL_SPI_STATE_BUSY_TX) {
		state = HAL_SPI_GetState(&s_spi);
	}

	uint8_t result;
	HAL_SPI_TransmitReceive(&s_spi, &data, &result, 1, HAL_MAX_DELAY);
	return result;
}
