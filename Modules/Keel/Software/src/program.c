#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"
#include "nrf24.h"
#include "protocol.h"
#include "configurablevalues.h"
#include "dynamicvalues.h"
#include "dendrite.h"

static void processPackage(PZ_Package const* p);
static void axonSend(PZ_Package const* p);
static void axonSendRaw(uint8_t const* data, uint8_t len);

static uint8_t s_on = 0;

static void processPackage(PZ_Package const* p) {
	PZ_Package r;
	switch (p->cmd) {
		case PZ_Cmd_Info:
			r = PZ_composeRe2(p, PZ_KEEL_TYPE, PZ_VERSION);
			axonSend(&r);
			break;
		case PZ_Cmd_Info_re:
			break;
		case PZ_Cmd_Read_cv:
			r = PZ_composeRe2(p, p->pld[0], readCv(p->pld[0]));
			axonSend(&r);
			break;
		case PZ_Cmd_Read_cv_re:
			break;
		case PZ_Cmd_Write_cv:
			writeCv(p->pld[0], &p->pld[1]);
			break;
		case PZ_Cmd_Reset_cv:
			resetCv(p->pld[0]);
			break;
		case PZ_Cmd_Reset_all_cv:
			resetAllCv();
			break;
		case PZ_Cmd_Read_dv:
			r = PZ_composeRe2(p, p->pld[0], readDv(p->pld[0]));
			axonSend(&r);
			break;
		case PZ_Cmd_Read_dv_re:
			break;
		case PZ_Cmd_Write_dv:
			// Nothing for this module
			break;
		case PZ_Cmd_Reset_dv:
			// Nothing for this module
			break;
		case PZ_Cmd_Reset_all_dv:
			// Nothing for this module
			break;
	}
}

static void axonSend(PZ_Package const* p) {
	uint8_t len = 0;
	uint8_t buffer[PZ_MAX_PACKAGE_LEN];
	PZ_toData(buffer, &len, p);
	axonSendRaw(buffer, len);
}

static void axonSendRaw(uint8_t const* data, uint8_t len) {
	NRF24_StopReceive();
	NRF24_WritePayload(data, len);
	NRF24_SetOperationalMode(NRF24_MODE_TX);
	uint8_t result = NRF24_Transmit();
	NRF24_SetOperationalMode(NRF24_MODE_RX);
	NRF24_FlushTX();
	NRF24_StartReceive();
	if (result == 0) {
		//printf("Transmission OK.\r\n");
	}
	else if (result == 1) {
		//printf("Transmission Failed: Retransmittion limit reached.\r\n");
	}
	else {
		//printf("Transmission Failed: Unkown error.\r\n");
	}
}

void onTimer() {
	if (s_on) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	}
}

void setup() {
	NRF24_Init(GetSpi());
	//printf("\r\n\r\n** [Keel] Built: %s %s **\r\n\n", __DATE__, __TIME__);

	GPIO_InitTypeDef port = {0};

	/*Configure GPIOA1 pin */
	port.Pin = GPIO_PIN_1;
	port.Mode = GPIO_MODE_OUTPUT_PP;
	port.Pull = GPIO_NOPULL;
	port.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &port);
	NRF24_Device_Init();
	if (!NRF24_Check()) {
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
	s_on = 1;
}

void poll() {
	const uint8_t hasSpiData = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
	if (hasSpiData) {
		uint8_t dataLen = 0;
		uint8_t buffer[PZ_MAX_PACKAGE_LEN];
		NRF24_ReadDynPayload(buffer, &dataLen);

		if (PZ_verify(buffer, dataLen) == PZ_OK) {
			PZ_Package package = PZ_fromData(buffer);

			if (package.adr == getAddress()) {
				processPackage(&package);
			}
			else {
				dendriteSend(&package);
			}
		}
		NRF24_ClearIRQFlags();
		NRF24_FlushRX();
	}
	const uint8_t dendriteDataLength = getDendriteDataLen();
	if (dendriteDataLength > 0) {
		axonSendRaw(getDendriteData(), dendriteDataLength);
		resetDendriteData();
	}
}
