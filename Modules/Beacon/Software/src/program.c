#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"
#include "printf.h"
#include "nrf24.h"
#include "terminal.h"
#include "protocol.h"
#include <stdlib.h>

static uint8_t s_uart_buff;
uint8_t s_on = 0;
PZ_Package s_pkg;

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
static void sendPz(PZ_Package* p) {
	uint8_t msg[8];
	uint8_t msgLen = 0;
	printf("<< ");
	PZ_PrintInfo(printf, p);
	PZ_toData(msg, &msgLen, p);
	send(msg, msgLen);
}

void cmd_info(char const* argv[], uint8_t argn) {
	if (argn > 1) {
		uint8_t adr = atoi(argv[1]);
		s_pkg = PZ_compose0(adr, PZ_Cmd_Info);
		sendPz(&s_pkg);
	}
	else {
		printf("Error. Format: info <adr>\r\n");
	}
}

void cmd_writeCv(char const* argv[], uint8_t argn) {
	if (argn > 3) {
		uint8_t adr = atoi(argv[1]);
		uint8_t cv = atoi(argv[2]);
		uint8_t val = atoi(argv[3]);
		s_pkg = PZ_compose2(adr, PZ_Cmd_Write_cv, cv, val);
		sendPz(&s_pkg);
	}
	else {
		printf("Error. Format: write-cv <adr> <cv> <val>\r\n");
	}
}

void cmd_readCv(char const* argv[], uint8_t argn) {
	if (argn > 1) {
		uint8_t adr = atoi(argv[1]);
		uint8_t cv = atoi(argv[2]);
		s_pkg = PZ_compose1(adr, PZ_Cmd_Read_cv, cv);
		sendPz(&s_pkg);
	}
	else {
		printf("Error. Format: read-cv <adr> <cv>\r\n");
	}
}

void cmd_on(char const* argv[], uint8_t argn) { s_on = 1; }
void cmd_off(char const* argv[], uint8_t argn) { s_on = 0; }

void onTimer() {
	if (s_on) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
		sendPz(&s_pkg);
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
	}
	NRF24_FlushRX();
}

void onUart() {
	TM_onChar(s_uart_buff);
	HAL_UART_Receive_IT(getUart(), &s_uart_buff, 1);
}

void setup() {
	TM_registerCommand("info", cmd_info);
	TM_registerCommand("on", cmd_on);
	TM_registerCommand("off", cmd_off);
	TM_registerCommand("write-cv", cmd_writeCv);
	TM_registerCommand("read-cv", cmd_readCv);

	s_pkg = PZ_compose2(0x10, PZ_Cmd_Write_dv, 0x10, 0x02);

	NRF24_Init(GetSpi());

	printf("\r\n\r\n** [Beacon] Built: %s %s **\r\n\n", __DATE__, __TIME__);
	HAL_UART_Receive_IT(getUart(), &s_uart_buff, 1);

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
		s_on = 0;
		return;
	}

	s_on = 0;

	uint8_t const clientAddr[] = PZ_CLIENT_ADDR;
	uint8_t const hostAddr[] = PZ_HOST_ADDR;
	NRF24_SetRFChannel(90); // set RF channel to 2490MHz
	NRF24_SetDataRate(NRF24_DR_2Mbps); // 2Mbit/s data rate
	NRF24_SetCRCScheme(NRF24_CRC_1byte); // 1-byte CRC scheme
	NRF24_SetAddrWidth(5); // address width is 5 bytes
	NRF24_SetAddr(NRF24_PIPETX, clientAddr);
	NRF24_SetAddr(NRF24_PIPE0, clientAddr);
	NRF24_SetAddr(NRF24_PIPE1, hostAddr); // program pipe address
	NRF24_SetRXPipe(NRF24_PIPE1, NRF24_AA_ON, 8); // enable RX pipe#1 with Auto-ACK: disabled, payload length: 10 bytes
	NRF24_LockUnlockFeature();
	NRF24_EableDynPl();
	NRF24_SetTXPower(NRF24_TXPWR_0dBm); // configure TX power
	NRF24_SetAutoRetr(NRF24_ARD_2500us, 10);
	NRF24_EnableAA(NRF24_PIPE0);
	NRF24_SetOperationalMode(NRF24_MODE_RX); // switch transceiver to the TX mode
	NRF24_SetIrqMask(NRF24_FLAG_RX_DR);
	NRF24_SetPowerMode(NRF24_PWR_UP); // wake-up transceiver (in case if it is sleeping)
	NRF24_StartReceive();
	//NRF24_DumpConfig(printf);
}

uint8_t PZ_crc(uint8_t const* data, uint8_t size) {
	return HAL_CRC_Calculate(GetCrc(), (uint32_t*)data, size);
}
