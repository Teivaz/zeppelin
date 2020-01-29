#include "dendrite.h"
#include "main.h"

static struct {
	uint8_t addr;
	uint8_t dataReady;
	uint8_t data[PZ_MAX_PACKAGE_LEN];
	uint8_t len;
} s_dendrite = {
	.addr = 0,
	.dataReady = 0,
	.len = 0,
};

static enum {
	EStateIdle,
	EStateTransmit1,
	EStateTransmit2,
	EStateTransmitRe1,
	EStateTransmitRe2,
	EStateReceive1,
	EStateReceive2,
	EStateAbort,
} s_state = EStateIdle;

uint8_t* getDendriteData() {
	return s_dendrite.data;
}

uint8_t getDendriteDataLen() {
	return s_dendrite.dataReady ? s_dendrite.len : 0;
}

void resetDendriteData() {
	s_dendrite.dataReady = 0;
}

void dendriteSend(PZ_Package const* package) {
	if (s_state == EStateIdle) {
		s_dendrite.dataReady = 0;
		s_dendrite.addr = package->adr;
		PZ_toData(s_dendrite.data, &s_dendrite.len, package);
		if (PZ_needsResponse(package)) {
			s_state = EStateTransmitRe1;
		}
		else {
			s_state = EStateTransmit1;
		}
		HAL_I2C_Master_Seq_Transmit_IT(GetI2c(), s_dendrite.addr, s_dendrite.data, PZ_HEADER_LEN, I2C_FIRST_AND_NEXT_FRAME);
	}
	else if (s_state == EStateAbort) {
	}
	else {
		s_state = EStateAbort;
		HAL_I2C_Master_Abort_IT(GetI2c(), s_dendrite.addr);
	}
}

// Read in 2 stages: header, remaining part
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c) {
	if (s_state == EStateTransmit1) {
		s_state = EStateTransmit2;
		HAL_I2C_Master_Seq_Transmit_IT(hi2c, s_dendrite.addr, &s_dendrite.data[PZ_HEADER_LEN], s_dendrite.len - PZ_HEADER_LEN, I2C_LAST_FRAME);
	}
	else if (s_state == EStateTransmitRe1) {
		s_state = EStateTransmitRe2;
		HAL_I2C_Master_Seq_Transmit_IT(hi2c, s_dendrite.addr, &s_dendrite.data[PZ_HEADER_LEN], s_dendrite.len - PZ_HEADER_LEN, I2C_LAST_FRAME_NO_STOP);
	}
	else if (s_state == EStateTransmit2) {
		s_state = EStateIdle;
	}
	else if (s_state == EStateTransmitRe2) {
		s_state = EStateReceive1;
		HAL_I2C_Master_Seq_Receive_IT(hi2c, s_dendrite.addr, s_dendrite.data, PZ_HEADER_LEN, I2C_FIRST_AND_NEXT_FRAME);
	}
	else if (s_state == EStateAbort) {
		s_state = EStateIdle;
	}
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c) {
	if (s_state == EStateReceive1) {
		s_state = EStateReceive2;
		const uint8_t len = s_dendrite.data[2];
		s_dendrite.len = len + PZ_HEADER_LEN;
		HAL_I2C_Master_Seq_Receive_IT(hi2c, s_dendrite.addr, &s_dendrite.data[PZ_HEADER_LEN], len, I2C_LAST_FRAME);
	}
	else if (s_state == EStateReceive2) {
		s_state = EStateIdle;
		s_dendrite.dataReady = 1;
	}
	else if (s_state == EStateAbort) {
		s_state = EStateIdle;
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* hi2c) {
	s_state = EStateAbort;
	HAL_I2C_Master_Abort_IT(GetI2c(), s_dendrite.addr);
}

void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef* hi2c) {
	if (s_state == EStateAbort) {
		s_state = EStateIdle;
	}
}
