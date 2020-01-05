#include "program.h"
#include "main.h"
#include "nrf24.h"

void setup() {
	NRF24_SetPowerMode(NRF24_PWR_UP);
	NRF24_SetOperationalMode(NRF24_MODE_TX);
	//NRF24_SetRXPipe(0, NRF24_AA_OFF, 32);
	NRF24_SetRXPipe(0, NRF24_AA_OFF, 8);
	//NRF24_SetRFChannel();
	NRF24_SetDataRate(NRF24_DR_1Mbps);

	// Wait for 2400 cycles at 16 MHz = 150us.
	for(int32_t i = 2400U; i > 0; --i)
	{
		__asm__ __volatile__ ("nop"::);
	}



	GPIO_InitTypeDef port = {0};

	/*Configure GPIOA1 pin */
	port.Pin = GPIO_PIN_1;
	port.Mode = GPIO_MODE_OUTPUT_PP;
	port.Pull = GPIO_NOPULL;
	port.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &port);

}

void onTimer() {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);

	uint8_t payload[8] = {
			(uint8_t)0x55, // Hdr
			(uint8_t)0x10, // default Adr
			(uint8_t)0x05, // Len
			(uint8_t)0x00, // RId
			(uint8_t)0x06, // Cmd - write-dv
			(uint8_t)0x10, // value of servo
			(uint8_t)0x02, // 2 out of 255
			(uint8_t)0x00, // CRC
	};
	NRF24_WritePayload(payload, sizeof(payload));
	NRF24_Transmit();
}


