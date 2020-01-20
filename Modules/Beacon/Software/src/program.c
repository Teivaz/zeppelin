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
	NRF24_WritePayload(data, len);
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
static void sendPz(PZ_Package* p) {
	uint8_t msg[8];
	uint8_t msgLen = 0;
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

void cmd_setCv(char const* argv[], uint8_t argn) {
	if (argn > 3) {
		uint8_t adr = atoi(argv[1]);
		uint8_t cv = atoi(argv[2]);
		uint8_t val = atoi(argv[3]);
		s_pkg = PZ_compose2(adr, PZ_Cmd_Write_cv, cv, val);
		sendPz(&s_pkg);
	}
	else {
		printf("Error. Format: set-cv <adr> <cv> <val>\r\n");
	}
}

void cmd_getCv(char const* argv[], uint8_t argn) {
	if (argn > 1) {
		uint8_t adr = atoi(argv[1]);
		uint8_t cv = atoi(argv[2]);
		s_pkg = PZ_compose1(adr, PZ_Cmd_Read_cv, cv);
		sendPz(&s_pkg);
	}
	else {
		printf("Error. Format: get-cv <adr> <cv>\r\n");
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

void toggleTimer() {
}
void onUart() {
	TM_onChar(s_uart_buff);
	HAL_UART_Receive_IT(getUart(), &s_uart_buff, 1);
}

void setup() {
	TM_registerCommand("info", cmd_info);
	TM_registerCommand("on", cmd_on);
	TM_registerCommand("off", cmd_off);
	TM_registerCommand("set-cv", cmd_setCv);
	TM_registerCommand("get-cv", cmd_getCv);

	s_pkg = PZ_compose2(0x10, PZ_Cmd_Write_dv, 0x10, 0x02);
	/*{
			(uint8_t)0x55, // Hdr
			(uint8_t)0x10, // default Adr
			(uint8_t)0x05, // Len
			(uint8_t)0x00, // RId
			(uint8_t)0x06, // Cmd - write-dv
			(uint8_t)0x10, // value of servo
			(uint8_t)0x02, // 2 out of 255
			(uint8_t)0x6b, // CRC - 0x6b
	};
	s_msgLen = 8;
	*/

	HAL_UART_Receive_IT(getUart(), &s_uart_buff, 1);

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

	s_on = 0;
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


	//NRF24_DumpConfig(printf);
}

uint8_t PZ_crc(uint8_t const* data, uint8_t size) {
	return HAL_CRC_Calculate(GetCrc(), (uint32_t*)data, size);
}
