#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"
#include "printf.h"
#include "nrf24.h"
#include "protocol.h"
#include "keel.h"

uint8_t s_on = 1;

static void send(uint8_t* data, uint8_t len) {
	NRF24_StopReceive();
	NRF24_WritePayload(data, len);
	NRF24_SetOperationalMode(NRF24_MODE_TX);
	uint8_t result = NRF24_Transmit();
	NRF24_SetOperationalMode(NRF24_MODE_RX);
	NRF24_StartReceive();
	if (result == 0) {
		//printf("Transmission OK.\r\n");
	}
	else if (result == 1) {
		printf("Transmission Failed: Retransmittion limit reached.\r\n");
	}
	else {
		printf("Transmission Failed: Unkown error.\r\n");
	}
}
void sendPz(PZ_Package* p) {
	uint8_t msg[8];
	uint8_t msgLen = 0;
	printf("<< ");
	PZ_PrintInfo(printf, p);
	PZ_toData(msg, &msgLen, p);
	send(msg, msgLen);
}

uint8_t PZ_crc(uint8_t const* data, uint8_t size) {
	return HAL_CRC_Calculate(GetCrc(), (uint32_t*)data, size);
}

void onTimer() {
	if (s_on) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	}
}

void onExtIrq() {
	NRF24_ClearIRQFlags();
	uint8_t payload[32];
	uint8_t len;
	NRF24_ReadDynPayload(payload, &len);
	if (PZ_verify(payload, len) == PZ_OK) {
		PZ_Package package = PZ_fromData(payload);
		printf(">> ");
		PZ_PrintInfo(printf, &package);
		processPackage(&package, payload, len);
	}
	else {
		printf("Not a valid package\r\n");
		if (len > 7) {
			printf("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
					payload[0], payload[1], payload[2], payload[3],
					payload[4], payload[5], payload[6], payload[7]
			);
		}
	}
	NRF24_FlushRX();
}

void setup() {
	NRF24_Init(GetSpi());
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

	uint8_t const clientAddr[] = PZ_CLIENT_ADDR;
	uint8_t const hostAddr[] = PZ_HOST_ADDR;
	NRF24_SetRFChannel(90); // set RF channel to 2490MHz
	NRF24_SetDataRate(NRF24_DR_2Mbps); // 2Mbit/s data rate
	NRF24_SetCRCScheme(NRF24_CRC_1byte); // 1-byte CRC scheme
	NRF24_SetAddrWidth(5); // address width is 5 bytes
	NRF24_SetAddr(NRF24_PIPETX, hostAddr);
	NRF24_SetAddr(NRF24_PIPE0, hostAddr); // program pipe address
	NRF24_SetAddr(NRF24_PIPE1, clientAddr); // program pipe address
	NRF24_SetRXPipe(NRF24_PIPE1, NRF24_AA_ON, 8); // enable RX pipe#1 with Auto-ACK: disabled, payload length: 10 bytes
	NRF24_LockUnlockFeature();
	NRF24_EableDynPl();
	NRF24_SetTXPower(NRF24_TXPWR_0dBm);
	NRF24_SetAutoRetr(NRF24_ARD_2500us, 10);
	NRF24_EnableAA(NRF24_PIPE0);
	NRF24_SetOperationalMode(NRF24_MODE_RX); // switch transceiver to the RX mode
	NRF24_SetIrqMask(NRF24_FLAG_RX_DR);
	NRF24_SetPowerMode(NRF24_PWR_UP); // wake-up transceiver (in case if it is sleeping)
	NRF24_StartReceive();
	//NRF24_DumpConfig(printf);
}

uint8_t getBattery0() {
	return 24;
}
uint8_t getBattery1() {
	return 13;
}
uint8_t getThermometer0() {
	return 12;
}
