#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"
#include "printf.h"
#include "nrf24.h"

uint8_t s_on = 1;

void onTimer() {
	if (s_on) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);

		uint8_t payload[32] = {
				(uint8_t)0x55, // Hdr
				(uint8_t)0x10, // default Adr
				(uint8_t)0x05, // Len
				(uint8_t)0x00, // RId
				(uint8_t)0x06, // Cmd - write-dv
				(uint8_t)0x10, // value of servo
				(uint8_t)0x02, // 2 out of 255
				(uint8_t)0x00, // CRC - 0x6b

				(uint8_t)0x00, // rest
		};
		payload[7] = HAL_CRC_Calculate(GetCrc(), (uint32_t*)payload, 7);
		NRF24_WritePayload(payload, 8);
		printf("Sending data.\r\n");
		uint8_t result = NRF24_Transmit();
		if (result == 0) {
			printf("Transmission OK.\r\n");
		}
		else if (result == 1) {
			printf("Transmission Failed: Retransmittion limit reached.\r\n");
		}
		else {
			printf("Transmission Failed: Unkown error.\r\n");
		}
	}
}

void toggleTimer() {
}

void setup() {
	NRF24_Init(GetSpi());

	printf("\r\n\r\n** [Beacon] Built: %s %s **\r\n\n", __DATE__, __TIME__);

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

	uint8_t addr[] = { 'n', 'R', 'F', '2', '4' }; // the TX address
	NRF24_SetRFChannel(90); // set RF channel to 2490MHz
	NRF24_SetDataRate(NRF24_DR_2Mbps); // 2Mbit/s data rate
	NRF24_SetCRCScheme(NRF24_CRC_1byte); // 1-byte CRC scheme
	NRF24_SetAddrWidth(5); // address width is 5 bytes
	NRF24_SetAddr(NRF24_PIPETX, addr);
	NRF24_SetAddr(NRF24_PIPE0, addr);
	NRF24_SetTXPower(NRF24_TXPWR_0dBm); // configure TX power
	NRF24_SetAutoRetr(NRF24_ARD_2500us, 10);
	NRF24_EnableAA(NRF24_PIPE0);
	NRF24_SetOperationalMode(NRF24_MODE_TX); // switch transceiver to the TX mode
	NRF24_SetPowerMode(NRF24_PWR_UP); // wake-up transceiver (in case if it sleeping)


	NRF24_DumpConfig(printf);
}

