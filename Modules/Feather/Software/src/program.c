#include "main.h"
#include "program.h"
#include "stm32l0xx_hal.h"
#include "protocol.h"
#include "configurablevalues.h"
#include "dynamicvalues.h"

static void send(PZ_Package* p);
static void processPackage(PZ_Package* p);

static uint8_t s_buffer[PZ_MAX_PACKAGE_LEN];
static uint8_t s_bufferLen = 0;

static void processPackage(PZ_Package* p) {
	if (p->adr != getAddress()) {
		return; // I2C ensures we don't get here
	}
	PZ_Package r;
	switch (p->cmd) {
		case PZ_Cmd_Info:
			r = PZ_composeRe2(p, PZ_FEATHER_TYPE, PZ_VERSION);
			send(&r);
			break;
		case PZ_Cmd_Info_re:
			break;
		case PZ_Cmd_Read_cv:
			r = PZ_composeRe2(p, p->pld[0], readCv(p->pld[0]));
			send(&r);
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
			send(&r);
			break;
		case PZ_Cmd_Read_dv_re:
			break;
		case PZ_Cmd_Write_dv:
			writeDv(p->pld[0], &p->pld[1]);
			break;
		case PZ_Cmd_Reset_dv:
			resetDv(p->pld[0]);
			break;
		case PZ_Cmd_Reset_all_dv:
			resetAllCv();
			break;
	}
}

static const uint16_t s_minServoTimerValue = 258; // Corresponds to 1ms
static const uint16_t s_maxServoTimerValue = 516; // Corresponds to 2ms

static uint16_t servoToTimerValue(uint8_t value) {
	if (getServoCalMode() == 0) {
		const uint32_t minimal = s_minServoTimerValue + getServoCal1() - 128;
		const uint32_t maximal = s_maxServoTimerValue + getServoCal2() - 128;
		const uint32_t range = maximal - minimal;
		const uint32_t normalized = (uint32_t)value * range / 0xff;
		return minimal + normalized;
	}
	else if (getServoCalMode() == 1) {
		return s_minServoTimerValue + value - 128;
	}
	else if (getServoCalMode() == 2) {
		return s_maxServoTimerValue + value - 128;
	}
	return s_minServoTimerValue;
}

static void _setServo(uint8_t value) {
	TIM21->CCR1 = servoToTimerValue(value);
}

void DvOnServoChanged(uint8_t value) {
	_setServo(value);
}
void CvOnServoCal1Changed(uint8_t value) {
	(void)value;
	_setServo(getServo());
}
void CvOnServoCal2Changed(uint8_t value) {
	(void)value;
	_setServo(getServo());
}
void DvOnServoCalModeChanged(uint8_t value) {
	switch (value) {
		case 0: // Normal mode
			setServo(getServoDef());
			break;
		case 1: // Calibrating minimal value
			setServo(getServoCal1());
			break;
		case 2: // Calibrating maximal value
			setServo(getServoCal2());
			break;
		default:
			break;
	}
	_setServo(getServo());
}
void CvOnAddressChanged(uint8_t value) {
	GetI2c()->Instance->OAR1 &= ~I2C_OAR1_OA1EN;
	GetI2c()->Instance->OAR1 = (I2C_OAR1_OA1EN | value);
}

void Error() {
	void __blocking_handler();
	__blocking_handler();
}

static void send(PZ_Package* p) {
	PZ_toData(s_buffer, &s_bufferLen, p);
}

uint8_t PZ_crc(uint8_t const* data, uint8_t size) {
	return HAL_CRC_Calculate(GetCrc(), (uint32_t*)data, size);
}

void onTimer() {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
}

void setup() {
	//printf("\r\n\r\n** [Feather] Built: %s %s **\r\n\n", __DATE__, __TIME__);

	GPIO_InitTypeDef port = {0};

	/*Configure GPIOA1 pin */
	port.Pin = GPIO_PIN_1;
	port.Mode = GPIO_MODE_OUTPUT_PP;
	port.Pull = GPIO_NOPULL;
	port.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &port);

	if (HAL_I2C_EnableListen_IT(GetI2c()) != HAL_OK) {
		Error();
	}
}

static enum {EStateIdle, EStateReceive1, EStateReceive2, EStateTransmit1, EStateTransmit2} s_state = EStateIdle;

void HAL_I2C_AddrCallback(I2C_HandleTypeDef* hi2c, uint8_t direction, uint16_t AddrMatchCode) { 
	if (direction == I2C_DIRECTION_TRANSMIT) {
		if (s_state == EStateIdle) {
			s_state = EStateReceive1;
			if (HAL_I2C_Slave_Seq_Receive_IT(hi2c, s_buffer, PZ_HEADER_LEN, I2C_FIRST_FRAME) != HAL_OK) {
				Error();
			}
		}
	}
	else if (direction == I2C_DIRECTION_RECEIVE) {
		if (s_state == EStateIdle) {
			s_state = EStateTransmit1;
			if (HAL_I2C_Slave_Seq_Transmit_IT(hi2c, s_buffer, PZ_HEADER_LEN, I2C_FIRST_FRAME) != HAL_OK) {
				Error();
			}
		}
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef* hi2c) {
	if (s_state == EStateReceive1) {
		const uint8_t restLen = s_buffer[2];
		s_bufferLen = PZ_HEADER_LEN + restLen;
		s_state = EStateReceive2;
		if (HAL_I2C_Slave_Seq_Receive_IT(hi2c, &s_buffer[PZ_HEADER_LEN], restLen, I2C_FIRST_FRAME) != HAL_OK) {
			Error();
		}
	}
	else if (s_state == EStateReceive2) {
		s_state = EStateIdle;
		if (PZ_verify(s_buffer, s_bufferLen) == PZ_OK) {
			PZ_Package package = PZ_fromData(s_buffer);
			processPackage(&package);
		}
	}
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef* hi2c) {
	if (s_state == EStateTransmit1) {
		s_state = EStateTransmit2;
		if (HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &s_buffer[PZ_HEADER_LEN], s_bufferLen - PZ_HEADER_LEN, I2C_LAST_FRAME) != HAL_OK) {
			Error();
		}
	}
	else if (s_state == EStateTransmit2) {
		s_state = EStateIdle;
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* hi2c) {
	s_state = EStateIdle;
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef* hi2c) {
	if (HAL_I2C_EnableListen_IT(hi2c) != HAL_OK) {
		Error();
	}
}

void poll() {
}
