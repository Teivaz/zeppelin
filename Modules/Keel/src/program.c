#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"
#include "nrf24.h"
#include "protocol.h"
#include "configurablevalues.h"
#include "dynamicvalues.h"
#include "dendrite.h"

#define VREFINT_CAL (*(uint16_t const*)0x1FF80078UL)
#define TEMP30_CAL (*(uint16_t const*)0x1FF8007AUL)
#define TEMP130_CAL (*(uint16_t const*)0x1FF8007EUL)
#define VDD_CALIB ((uint16_t) (300)) // centivolts
#define ADC_MAX_VAL (uint16_t)0xFFF
#define V1ADC_R1 (uint16_t)51 // R1 - 51 kOhm
#define V1ADC_R2 (uint16_t)10 // R2 - 10 kOhm

static void processPackage(PZ_Package const* p);
static void axonSend(PZ_Package const* p);
static void axonSendRaw(uint8_t const* data, uint8_t len);
static uint8_t convertVoltage(uint16_t measure, uint8_t scaleFactor);
static uint8_t convertVref(uint16_t measure);
static int8_t convertTemp(uint16_t measure);

static uint8_t s_on = 0;
static uint16_t s_vref = 300; // centivolts
static uint8_t const s_selfAddress[] = PZ_CLIENT_ADDR;
static uint8_t const s_otherAddress[] = PZ_HOST_ADDR;
static NRF24_InstanceTypedef s_nrf24;

#pragma pack(push)
#pragma pack(2)
static union {
	uint16_t buffer[3];
	struct {
		uint16_t chan0; // ADC_CHANNEL_0
		uint16_t vref; // ADC_CHANNEL_VREFINT = ADC_CHANNEL_17
		uint16_t temp; // ADC_CHANNEL_TEMPSENSOR = ADC_CHANNEL_18
	};
} s_dmaAdc;
#pragma pack(pop)

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
			//writeDv(p->pld[0], &p->pld[1]); // Empty for this module
			break;
		case PZ_Cmd_Reset_dv:
			//resetDv(p->pld[0]); // Empty for this module
			break;
		case PZ_Cmd_Reset_all_dv:
			//resetAllDv(); // Empty for this module
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
	NRF24_Transmit_IT(&s_nrf24, data, len);
}

void onTimer() {
	if (s_on) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	}
}

static uint8_t convertVoltage(uint16_t measure, uint8_t scaleFactor) {
	uint16_t voltage = scaleFactor * measure * (int32_t)s_vref * (V1ADC_R1 + V1ADC_R2) / V1ADC_R2 / ADC_MAX_VAL; // centivolts
	voltage = (voltage + 5) / 10; // Round and convert to decivolts
	return (uint8_t)voltage;
}

// According to datasheet this is the way
static uint8_t convertVref(uint16_t measure) {
	// 3V x VREFINT_CAL / VREFINT_DATA
	s_vref = VDD_CALIB * VREFINT_CAL /  measure; // centivolts
	uint16_t const decivolts = (s_vref + 5) / 10;
	return (uint8_t)decivolts; // Round and convert to decivolts
}

// According to datasheet this is the way
static int8_t convertTemp(uint16_t measure) {
	int32_t temperature;
	temperature = ((measure * (int32_t)s_vref / VDD_CALIB) - TEMP30_CAL);
	temperature = temperature * (130 - 30);
	temperature = temperature / (TEMP130_CAL - TEMP30_CAL);
	temperature = temperature + 30;
	return temperature;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	setBat0(convertVref(s_dmaAdc.vref));
	setBat1(convertVoltage(s_dmaAdc.chan0, 1));
	setTemp0(convertTemp(s_dmaAdc.temp));
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

	if (NRF24_Check()) {
		NRF24_Device_Init();

		// Common setup
		s_nrf24.init.rfChannel = 90; // 90 => 2490MHz
		s_nrf24.init.dataRate = NRF24_DR_2Mbps;
		s_nrf24.init.txPower = NRF24_TXPWR_0dBm;
		s_nrf24.init.crcScheme = NRF24_CRC_1byte;
		s_nrf24.init.retransmitDelay = NRF24_ARD_500us;
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

	HAL_ADC_Start_DMA(GetAdc(), (uint32_t*)s_dmaAdc.buffer, 3);
}
void NRF24_OnSendErrorCallback() {
	NRF24_Receive_IT(&s_nrf24);
}

void NRF24_OnDataSentCallback() {
	NRF24_Receive_IT(&s_nrf24);
}

void NRF24_OnReceiveCallback(uint8_t const* data, uint8_t length, uint8_t pipe) {
	if (PZ_verify(data, length) == PZ_OK) {
		PZ_Package package = PZ_fromData(data);

		if (package.adr == getAddress()) {
			processPackage(&package);
		}
		else {
			dendriteSend(&package);
		}
	}
}

void poll() {
	uint8_t const hasSpiData = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
	if (hasSpiData) {
		NRF24_IrqHandler(&s_nrf24);
	}
	uint8_t const dendriteDataLength = getDendriteDataLen();
	if (dendriteDataLength > 0) {
		axonSendRaw(getDendriteData(), dendriteDataLength);
		resetDendriteData();
	}
}
