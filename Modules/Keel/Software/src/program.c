#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"
#include "printf.h"
#include "nrf24.h"

uint8_t s_on = 1;

void onTimer() {
	if (s_on) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	}
}

void onExtIrq() {
	NRF24_ClearIRQFlags();
	uint8_t payload[32];
	uint8_t len;
	NRF24_ReadPayload(payload, &len);
	printf("nrf24 received %i bytes\n\r", len);
	if (len > 7) {
		printf("nrf24 data:\n\r0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
				payload[0], payload[1], payload[2], payload[3],
				payload[4], payload[5], payload[6], payload[7]

		);
	}
	NRF24_FlushRX();
}

void setup() {
	NRF24_Init();
	printf("\r\n\r\n** [Keel] Built: %s %s **\r\n\n", __DATE__, __TIME__);

	GPIO_InitTypeDef port = {0};

	/*Configure GPIOA1 pin */
	port.Pin = GPIO_PIN_1;
	port.Mode = GPIO_MODE_OUTPUT_PP;
	port.Pull = GPIO_NOPULL;
	port.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &port);
	NRF24_Device_Init();
	const uint8_t check = NRF24_Check();
	if (check) {
		printf("NRF24 Device is OK\r\n");
		s_on = 1;
	} else {
		printf("NRF24 Devices is FAILED\r\n");
		printf("skipping config\r\n");
		s_on = 0;
		return;
	}

	uint8_t addr[] = { 'n', 'R', 'F', '2', '4' }; // the address for RX pipe
	NRF24_DisableAA(0xFF); // disable ShockBurst
	NRF24_SetRFChannel(90); // set RF channel to 2490MHz
	NRF24_SetDataRate(NRF24_DR_2Mbps); // 2Mbit/s data rate
	NRF24_SetCRCScheme(NRF24_CRC_1byte); // 1-byte CRC scheme
	NRF24_SetAddrWidth(5); // address width is 5 bytes
	NRF24_SetAddr(NRF24_PIPE1, addr); // program pipe address
	NRF24_SetRXPipe(NRF24_PIPE1, NRF24_AA_ON, 8); // enable RX pipe#1 with Auto-ACK: disabled, payload length: 10 bytes
	NRF24_SetTXPower(NRF24_TXPWR_0dBm);
	NRF24_SetOperationalMode(NRF24_MODE_RX); // switch transceiver to the RX mode

	NRF24_SetIrqMask(1, 0, 0);

	NRF24_SetPowerMode(NRF24_PWR_UP); // wake-up transceiver (in case if it sleeping)
	// then pull CE pin to HIGH, and the nRF24 will start a receive...

	NRF24_StartReceive();

	NRF24_DumpConfig(printf);
}

