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
static uint8_t s_hasSpiData = 0;
static uint8_t const s_selfAddress[] = PZ_HOST_ADDR;
static uint8_t const s_otherAddress[] = PZ_CLIENT_ADDR;
static NRF24_InstanceTypedef s_nrf24;
PZ_Package s_pkg;

static void send(uint8_t* data, uint8_t len) {
	NRF24_Transmit_IT(&s_nrf24, data, len);
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
		uint8_t adr = strtol(argv[1], 0, 16);
		s_pkg = PZ_compose0(adr, PZ_Cmd_Info);
		sendPz(&s_pkg);
	}
	else {
		printf("Error. Format: info <adr>\r\n");
	}
}

void cmd_writeCv(char const* argv[], uint8_t argn) {
	if (argn > 3) {
		uint8_t adr = strtol(argv[1], 0, 16);
		uint8_t cv = strtol(argv[2], 0, 16);
		uint8_t val = strtol(argv[3], 0, 16);
		s_pkg = PZ_compose2(adr, PZ_Cmd_Write_cv, cv, val);
		sendPz(&s_pkg);
	}
	else {
		printf("Error. Format: write-cv <adr> <cv> <val>\r\n");
	}
}

void cmd_readCv(char const* argv[], uint8_t argn) {
	if (argn > 1) {
		uint8_t adr = strtol(argv[1], 0, 16);
		uint8_t cv = strtol(argv[2], 0, 16);
		s_pkg = PZ_compose1(adr, PZ_Cmd_Read_cv, cv);
		sendPz(&s_pkg);
	}
	else {
		printf("Error. Format: read-cv <adr> <cv>\r\n");
	}
}

void cmd_readDv(char const* argv[], uint8_t argn) {
	if (argn > 1) {
		uint8_t adr = strtol(argv[1], 0, 16);
		uint8_t dv = strtol(argv[2], 0, 16);
		s_pkg = PZ_compose1(adr, PZ_Cmd_Read_dv, dv);
		sendPz(&s_pkg);
	}
	else {
		printf("Error. Format: read-dv <adr> <cv>\r\n");
	}
}

void cmd_writeDv(char const* argv[], uint8_t argn) {
	if (argn > 3) {
		uint8_t adr = strtol(argv[1], 0, 16);
		uint8_t dv = strtol(argv[2], 0, 16);
		uint8_t val = strtol(argv[3], 0, 16);
		s_pkg = PZ_compose2(adr, PZ_Cmd_Write_dv, dv, val);
		sendPz(&s_pkg);
	}
	else {
		printf("Error. Format: write-dv <adr> <cv> <val>\r\n");
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
	s_hasSpiData = 1;
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
	TM_registerCommand("write-dv", cmd_writeDv);
	TM_registerCommand("read-cv", cmd_readCv);
	TM_registerCommand("read-dv", cmd_readDv);

	s_pkg = PZ_compose1(0x10, PZ_Cmd_Read_dv, 0x10);

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
	if (!NRF24_Check()) {
		printf("NRF24 Devices is FAILED\r\n");
		return;
	}
	NRF24_Device_Init();

	printf("NRF24 Device is OK\r\n");

	// Common setup
	s_nrf24.init.rfChannel = 90; // 90 => 2490MHz
	s_nrf24.init.dataRate = NRF24_DR_1Mbps;
	s_nrf24.init.txPower = NRF24_TXPWR_0dBm;
	s_nrf24.init.crcScheme = NRF24_CRC_1byte;
	s_nrf24.init.retransmitDelay = 
	s_nrf24.init.maxRetransmits = 10;
	s_nrf24.init.addrLen = 5;
	s_nrf24.init.selfAddr = s_selfAddress;
	s_nrf24.init.otherAddr = s_otherAddress;
	s_nrf24.mode = NRF24_MODE_RX;

	if (NRF24_init(&s_nrf24) != NRF24_Error_OK) {
		//Error handler
	}
	if (NRF24_Receive_IT(&s_nrf24) != NRF24_Error_OK) {
		//Error handler
	}
	s_on = 1;
}

void poll() {
	if (s_hasSpiData) {
		s_hasSpiData = 0;
		NRF24_IrqHandler(&s_nrf24);
	}
}
void NRF24_OnSendErrorCallback() {
	printf("Transmission Failed: Retransmittion limit reached.\r\n");
	NRF24_Receive_IT(&s_nrf24);
}

void NRF24_OnDataSentCallback() {
	NRF24_Receive_IT(&s_nrf24);
}

void NRF24_OnReceiveCallback(uint8_t const* data, uint8_t length, uint8_t pipe) {
		if (PZ_verify(data, length) == PZ_OK) {
			PZ_Package package = PZ_fromData(data);
			printf(">> ");
			PZ_PrintInfo(printf, &package);
		}
}
