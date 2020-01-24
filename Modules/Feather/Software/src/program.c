#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"
#include "printf.h"
#include "protocol.h"
#include "configurablevalues.h"

void processPackage(PZ_Package* p, uint8_t* raw, uint8_t size) {
	if (p->adr == getAddress()) {
		if (p->cmd == PZ_Cmd_Info) {
			PZ_Package r = PZ_composeRe2(p, PZ_FEATHER_TYPE, PZ_VERSION);
			sendPz(&r);
		}
		else if (p->cmd == PZ_Cmd_Read_cv) {
			const uint8_t val = readCv(p->pld[0]);
			PZ_Package r = PZ_composeRe2(p, p->pld[0], val);
			sendPz(&r);
		}
		else if (p->cmd == PZ_Cmd_Write_cv) {
			writeCv(p->pld[0], &p->pld[1]);
		}

	}
}

static void send(uint8_t* data, uint8_t len) {
	//HAL_I2C_Master_Transmit(GetI2c(), p->adr, raw, size, HAL_MAX_DELAY);
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
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
}

uint8_t s_dp[6];

void setup() {
	printf("\r\n\r\n** [Feather] Built: %s %s **\r\n\n", __DATE__, __TIME__);

	GPIO_InitTypeDef port = {0};

	/*Configure GPIOA1 pin */
	port.Pin = GPIO_PIN_1;
	port.Mode = GPIO_MODE_OUTPUT_PP;
	port.Pull = GPIO_NOPULL;
	port.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &port);

	HAL_I2C_Slave_Receive_IT(GetI2c(), s_dp, 6);
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
