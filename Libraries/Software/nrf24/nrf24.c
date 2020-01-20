// Functions to manage the NRF24L01+ transceiver

#include "nrf24.h"
#include "nrf24_hal.h"

// The address used to test presence of the transceiver,
// note: should not exceed 5 bytes
#define NRF24_TEST_ADDR "nRF24"

// Addresses of the address registers
static const uint8_t NRF24_ADDR_REGS[7] = {
		NRF24_REG_RX_ADDR_P0,
		NRF24_REG_RX_ADDR_P1,
		NRF24_REG_RX_ADDR_P2,
		NRF24_REG_RX_ADDR_P3,
		NRF24_REG_RX_ADDR_P4,
		NRF24_REG_RX_ADDR_P5,
		NRF24_REG_TX_ADDR
};


// Reads a value of register
// input:
//   reg - number of register to read
// return: value of register
static uint8_t NRF24_ReadReg(uint8_t reg) {
	uint8_t value;

	NRF24_CSN_Low();
	NRF24_LL_RW(reg & NRF24_MASK_REG_MAP);
	value = NRF24_LL_RW(NRF24_CMD_NOP);
	NRF24_CSN_High();

	return value;
}

// Writes a new value to the register
// input:
//   reg - number of register to write
//   value - value to write
static void NRF24_WriteReg(uint8_t reg, uint8_t value) {
	NRF24_CSN_Low();
	if (reg < NRF24_CMD_W_REGISTER) {
		// This is a register access
		NRF24_LL_RW(NRF24_CMD_W_REGISTER | (reg & NRF24_MASK_REG_MAP));
		NRF24_LL_RW(value);
	} else {
		// This is a single byte command or future command/register
		NRF24_LL_RW(reg);
		if ((reg != NRF24_CMD_FLUSH_TX) && (reg != NRF24_CMD_FLUSH_RX) && \
				(reg != NRF24_CMD_REUSE_TX_PL) && (reg != NRF24_CMD_NOP)) {
			// Send register value
			NRF24_LL_RW(value);
		}
	}
	NRF24_CSN_High();
}

// Reads to the buffer from a multi-byte register
// input:
//   reg - number of register to read
//   pBuf - pointer to the buffer for register data
//   count - number of bytes to read
static void NRF24_ReadMBReg(uint8_t reg, uint8_t *pBuf, uint8_t count) {
	NRF24_CSN_Low();
	NRF24_LL_RW(reg);
	while (count--) { *pBuf++ = NRF24_LL_RW(NRF24_CMD_NOP); }
	NRF24_CSN_High();
}

// Writes a buffer to a multi-byte register
// input:
//   reg - number of register to write
//   pBuf - pointer to the buffer with data to write
//   count - number of bytes to write
static void NRF24_WriteMBReg(uint8_t reg, uint8_t *pBuf, uint8_t count) {
	NRF24_CSN_Low();
	NRF24_LL_RW(reg);
	while (count--) {
		NRF24_LL_RW(*pBuf++);
	}
	NRF24_CSN_High();
}

void NRF24_Init(void* spi) {
	NRF24_GPIO_Init();
	NRF24_SPI_Init(spi);
	// Send without activating CSN to set CLK to the right state
	NRF24_LL_RW(0x00);
}

// Configures the transceiver to its initial state
void NRF24_Device_Init() {
	//NRF24_CE_High();
	// Write to the registers their initial values
	NRF24_WriteReg(NRF24_REG_CONFIG,     0x08);
	NRF24_WriteReg(NRF24_REG_EN_AA,      0x3F);
	NRF24_WriteReg(NRF24_REG_EN_RXADDR,  0x03);
	NRF24_WriteReg(NRF24_REG_SETUP_AW,   0x03);
	NRF24_WriteReg(NRF24_REG_SETUP_RETR, 0x03);
	NRF24_WriteReg(NRF24_REG_RF_CH,      0x02);
	NRF24_WriteReg(NRF24_REG_RF_SETUP,   0x0E);
	NRF24_WriteReg(NRF24_REG_STATUS,     0x00);
	NRF24_WriteReg(NRF24_REG_RX_PW_P0,   0x00);
	NRF24_WriteReg(NRF24_REG_RX_PW_P1,   0x00);
	NRF24_WriteReg(NRF24_REG_RX_PW_P2,   0x00);
	NRF24_WriteReg(NRF24_REG_RX_PW_P3,   0x00);
	NRF24_WriteReg(NRF24_REG_RX_PW_P4,   0x00);
	NRF24_WriteReg(NRF24_REG_RX_PW_P5,   0x00);
	NRF24_WriteReg(NRF24_REG_DYNPD,      0x00);
	NRF24_WriteReg(NRF24_REG_FEATURE,    0x00);

	// Configure the default RX/TX address values
	uint8_t addr[5];
	uint8_t idx;
	for (idx = 0U; idx < sizeof(addr); idx++) {
		addr[idx] = 0xE7;
	}
	NRF24_SetAddr(NRF24_PIPETX, addr);
	NRF24_SetAddr(NRF24_PIPE0, addr);
	for (idx = 0U; idx < sizeof(addr); idx++) {
		addr[idx] = 0xC2;
	}
	NRF24_SetAddr(NRF24_PIPE1, addr);
	for (idx = 2U; idx < 6U; idx++) {
		addr[0] = idx + 0xC1;
		NRF24_SetAddr(idx, addr);
	}

	// Clear the FIFO's
	NRF24_FlushRX();
	NRF24_FlushTX();

	// Clear any pending interrupt flags
	NRF24_ClearIRQFlags();

	// Deassert CSN pin (chip release)
	NRF24_CSN_High();
}

// Checks the presence of the NRF24L01
// return:
//   zero - transceiver is absent
//   non-zero: transceiver is present
uint8_t NRF24_Check(void) {
	uint8_t rxbuf[sizeof(NRF24_TEST_ADDR) - 1U];
	uint8_t *ptr = (uint8_t *)NRF24_TEST_ADDR;
	uint8_t idx;

	// Write the test address to the TX_ADDR register
	NRF24_WriteMBReg(
		NRF24_CMD_W_REGISTER | NRF24_REG_TX_ADDR,
		ptr,
		sizeof(NRF24_TEST_ADDR) - 1U
	);

	// Read it back to the buffer
	NRF24_ReadMBReg(
		NRF24_CMD_R_REGISTER | NRF24_REG_TX_ADDR,
		rxbuf,
		sizeof(NRF24_TEST_ADDR) - 1U
	);

	// Compare transmitted and received data...
	for (idx = 0U; idx < sizeof(NRF24_TEST_ADDR) - 1U; idx++) {
		if (rxbuf[idx] != *ptr++) {
			// The transceiver is absent
			return 0U;
		}
	}

	// The transceiver is present
	return !0U;
}

// Set transceiver power mode
// input:
//   mode - new state of power mode, one of NRF24_PWR_xx values
void NRF24_SetPowerMode(uint8_t mode) {
	uint8_t reg;

	reg = NRF24_ReadReg(NRF24_REG_CONFIG);
	if (mode == NRF24_PWR_UP) {
		// Set the PWR_UP bit of CONFIG register to wake the transceiver
		// It goes into Standby-I mode with consumption about 26uA
		reg |= NRF24_CONFIG_PWR_UP;
	} else {
		// Clear the PWR_UP bit of CONFIG register to put the transceiver
		// It goes into Power Down mode with consumption about 900nA
		reg &= ~NRF24_CONFIG_PWR_UP;
	}
	NRF24_WriteReg(NRF24_REG_CONFIG, reg);
}

// Set transceiver operational mode
// input:
//   mode - operational mode, one of NRF24_MODE_xx values
void NRF24_SetOperationalMode(uint8_t mode) {
	uint8_t reg;

	// Configure PRIM_RX bit of the CONFIG register
	reg = NRF24_ReadReg(NRF24_REG_CONFIG);
	reg &= ~NRF24_CONFIG_PRIM_RX;
	reg |= (mode & NRF24_CONFIG_PRIM_RX);
	NRF24_WriteReg(NRF24_REG_CONFIG, reg);
}

// Configure transceiver CRC scheme
// input:
//   scheme - CRC scheme, one of NRF24_CRC_xx values
// note: transceiver will forcibly turn on the CRC in case if auto acknowledgment
//       enabled for at least one RX pipe
void NRF24_SetCRCScheme(uint8_t scheme) {
	uint8_t reg;

	// Configure EN_CRC[3] and CRCO[2] bits of the CONFIG register
	reg = NRF24_ReadReg(NRF24_REG_CONFIG);
	reg &= ~NRF24_MASK_CRC;
	reg |= (scheme & NRF24_MASK_CRC);
	NRF24_WriteReg(NRF24_REG_CONFIG, reg);
}

// Set RF frequency channel
// input:
//   channel - radio frequency channel, can be a value from 0 to 127
// note: frequency will be (2400 + channel)MHz
// note: PLOS_CNT[7:4] bits of the OBSERVER_TX register will be reset
void NRF24_SetRFChannel(uint8_t channel) {
	NRF24_WriteReg(NRF24_REG_RF_CH, channel);
}

// Set automatic retransmission parameters
// input:
//   ard - auto retransmit delay, one of NRF24_ARD_xx values
//   arc - count of auto retransmits, value form 0 to 15
// note: in order to disable automatic retransmission,
//       the value of the variable XXX should be zero
void NRF24_SetAutoRetr(uint8_t ard, uint8_t arc) {
	NRF24_WriteReg(
		NRF24_REG_SETUP_RETR,
		(uint8_t)((ard << 4) | (arc & NRF24_MASK_RETR_ARC))
	);
}

// Set of address widths
// input:
//   addr_width - RX/TX address field width, value from 3 to 5
// note: this setting is common for all pipes
void NRF24_SetAddrWidth(uint8_t addr_width) {
	NRF24_WriteReg(NRF24_REG_SETUP_AW, addr_width - 2U);
}

// Set static RX address for a specified pipe
// input:
//   pipe - pipe to configure address, one of NRF24_PIPEx values
//   addr - pointer to the buffer with address
// note: pipe can be a number from 0 to 5 (RX pipes) and 6 (TX pipe)
// note: the length of the buffer for pipes 0 and 1 must not be less
//       of the actual address width configured in the transceiver
// note: for the pipes[2..5] only the first byte from the addr buffer
//       will be written since the pipes[1..5] share the four
//       most significant address bytes
// note: always ensure that none of the data pipes have the same address
void NRF24_SetAddr(uint8_t pipe, const uint8_t *addr) {
	uint8_t addr_width;

	// RX_ADDR_Px register
	switch (pipe) {
		case NRF24_PIPETX:
		case NRF24_PIPE0:
		case NRF24_PIPE1:
			// Get address width
			addr_width = NRF24_GetAddrWidth();
#if (!NRF24_ADDR_REVERSE)
			NRF24_WriteMBReg(
				NRF24_CMD_W_REGISTER | NRF24_ADDR_REGS[pipe],
				(uint8_t *)addr,
				addr_width
			);
#else
			// Write address in reverse order
			NRF24_CSN_Low();
			NRF24_LL_RW(NRF24_CMD_W_REGISTER | NRF24_ADDR_REGS[pipe]);
			while (addr_width--) { NRF24_LL_RW(*(addr + addr_width)); }
			NRF24_CSN_High();
#endif // NRF24_ADDR_REVERSE
			break;
		case NRF24_PIPE2:
		case NRF24_PIPE3:
		case NRF24_PIPE4:
		case NRF24_PIPE5:
			// Write first byte from the addr buffer,
			// it will be the LSByte of the pipe address
			NRF24_WriteReg(NRF24_ADDR_REGS[pipe], *addr);
			break;
		default:
			// Incorrect pipe number -> do nothing
			break;
	}
}

// Configure RF output power in TX mode
// input:
//   tx_pwr - RF output power, one of NRF24_TXPWR_xx values
void NRF24_SetTXPower(uint8_t tx_pwr) {
	uint8_t reg;

	// Configure RF_PWR[2:1] bits of the RF_SETUP register
	reg = NRF24_ReadReg(NRF24_REG_RF_SETUP);
	reg &= ~NRF24_MASK_RF_PWR;
	reg |= tx_pwr;
	NRF24_WriteReg(NRF24_REG_RF_SETUP, reg);
}

// Configure transceiver data rate
// input:
//   data_rate - data rate, one of NRF24_DR_xx values
void NRF24_SetDataRate(uint8_t data_rate) {
	uint8_t reg;

	// Configure RF_DR_LOW[5] and RF_DR_HIGH[3] bits of the RF_SETUP register
	reg = NRF24_ReadReg(NRF24_REG_RF_SETUP);
	reg &= ~NRF24_MASK_DATARATE;
	reg |= data_rate;
	NRF24_WriteReg(NRF24_REG_RF_SETUP, reg);
}

// Configure a specified RX pipe
// input:
//   pipe - number of the RX pipe, value from 0 to 5
//   aa_state - state of auto acknowledgment, one of NRF24_AA_xx values
//   payload_len - payload length in bytes
void NRF24_SetRXPipe(uint8_t pipe, uint8_t aa_state, uint8_t payload_len) {
	uint8_t reg;

	// Enable the specified pipe (EN_RXADDR register)
	reg = (NRF24_ReadReg(NRF24_REG_EN_RXADDR) | (1U << pipe)) & NRF24_MASK_EN_RX;
	NRF24_WriteReg(NRF24_REG_EN_RXADDR, reg);

	// Set RX payload length (RX_PW_Px register)
	NRF24_WriteReg(NRF24_REG_RX_PW_P0 + pipe, payload_len & NRF24_MASK_RX_PW);

	// Set auto acknowledgment for a specified pipe (EN_AA register)
	reg = NRF24_ReadReg(NRF24_REG_EN_AA);
	if (aa_state == NRF24_AA_ON) {
		reg |= (1U << pipe);
	} else {
		reg &= ~(1U << pipe);
	}
	NRF24_WriteReg(NRF24_REG_EN_AA, reg);
}

// Disable specified RX pipe
// input:
//   PIPE - number of RX pipe, value from 0 to 5
void NRF24_ClosePipe(uint8_t pipe) {
	uint8_t reg;

	reg = NRF24_ReadReg(NRF24_REG_EN_RXADDR);
	reg &= ~(1U << pipe);
	reg &= NRF24_MASK_EN_RX;
	NRF24_WriteReg(NRF24_REG_EN_RXADDR, reg);
}

// Enable the auto retransmit (a.k.a. enhanced ShockBurst) for the specified RX pipe
// input:
//   pipe - number of the RX pipe, value from 0 to 5
void NRF24_EnableAA(uint8_t pipe) {
	uint8_t reg;

	// Set bit in EN_AA register
	reg = NRF24_ReadReg(NRF24_REG_EN_AA);
	reg |= (1U << pipe);
	NRF24_WriteReg(NRF24_REG_EN_AA, reg);
}

// Disable the auto retransmit (a.k.a. enhanced ShockBurst) for one or all RX pipes
// input:
//   pipe - number of the RX pipe, value from 0 to 5, any other value will disable AA for all RX pipes
void NRF24_DisableAA(uint8_t pipe) {
	if (pipe > 5U) {
		// Disable Auto-ACK for ALL pipes
		NRF24_WriteReg(NRF24_REG_EN_AA, 0x00);
	} else {
		// Clear bit in the EN_AA register
		uint8_t reg;
		reg = NRF24_ReadReg(NRF24_REG_EN_AA);
		reg &= ~(1U << pipe);
		NRF24_WriteReg(NRF24_REG_EN_AA, reg);
	}
}

void NRF24_EnableFeature(uint8_t features) {
	uint8_t reg;
	reg = NRF24_ReadReg(NRF24_REG_FEATURE);
	reg |= features;
	NRF24_WriteReg(NRF24_REG_FEATURE, reg);
}

void NRF24_DisableFeature(uint8_t features) {
	uint8_t reg;
	reg = NRF24_ReadReg(NRF24_REG_FEATURE);
	reg &= ~(1U << features);
	NRF24_WriteReg(NRF24_REG_FEATURE, reg);
}

void NRF24_EableDynPl() {
	NRF24_EnableFeature(NRF24_FEATURE_DPL);
	uint8_t reg;
	reg = NRF24_ReadReg(NRF24_REG_DYNPD);
	reg |= (1U << NRF24_PIPE0) | (1U << NRF24_PIPE1) | (1U << NRF24_PIPE2) | (1U << NRF24_PIPE3) | (1U << NRF24_PIPE4) | (1U << NRF24_PIPE5);
	NRF24_WriteReg(NRF24_REG_DYNPD, reg);
}

void NRF24_DisableDynPl() {
	NRF24_DisableFeature(NRF24_FEATURE_DPL);
	uint8_t reg;
	reg = NRF24_ReadReg(NRF24_REG_DYNPD);
	reg &= ~((1U << NRF24_PIPE0) | (1U << NRF24_PIPE1) | (1U << NRF24_PIPE2) | (1U << NRF24_PIPE3) | (1U << NRF24_PIPE4) | (1U << NRF24_PIPE5));
	NRF24_WriteReg(NRF24_REG_DYNPD, reg);
}

void NRF24_SetIrqMask(uint8_t rx_dr_enable, uint8_t tx_ds_enable, uint8_t max_rt_enable) {
	uint8_t reg = NRF24_ReadReg(NRF24_REG_CONFIG);
	reg &= ~(NRF24_FLAG_RX_DR | NRF24_FLAG_TX_DS | NRF24_FLAG_MAX_RT);
	reg |= ((!rx_dr_enable) & NRF24_FLAG_RX_DR) | ((!tx_ds_enable) & NRF24_FLAG_TX_DS) | ((!max_rt_enable) & NRF24_FLAG_MAX_RT);
	NRF24_WriteReg(NRF24_REG_CONFIG, reg);
}

void NRF24_LockUnlockFeature() {
	NRF24_CSN_Low();
	NRF24_LL_RW(NRF24_CMD_LOCK_UNLOCK);
	NRF24_LL_RW(0x73);
	NRF24_CSN_High();
}

// Get address length
// return: the length of the address configured in the transceiver,
//         value from 3 to 5
uint8_t NRF24_GetAddrWidth(void) {
	return NRF24_ReadReg(NRF24_REG_SETUP_AW) + 2U;
}

// Get value of the STATUS register
// return: value of STATUS register
uint8_t NRF24_GetStatus(void) {
	return NRF24_ReadReg(NRF24_REG_STATUS);
}

// Get pending IRQ flags
// return: current status of RX_DR, TX_DS and MAX_RT bits of the STATUS register
uint8_t NRF24_GetIRQFlags(void) {
	return (NRF24_ReadReg(NRF24_REG_STATUS) & NRF24_MASK_STATUS_IRQ);
}

// Get status of the RX FIFO
// return: one of the NRF24_STATUS_RXFIFO_xx values
uint8_t NRF24_GetStatus_RXFIFO(void) {
	return (NRF24_ReadReg(NRF24_REG_FIFO_STATUS) & NRF24_MASK_RXFIFO);
}

// Get status of the TX FIFO
// return: one of the NRF24_STATUS_TXFIFO_xx values
// note: the TX_REUSE bit ignored
uint8_t NRF24_GetStatus_TXFIFO(void) {
	return ((NRF24_ReadReg(NRF24_REG_FIFO_STATUS) & NRF24_MASK_TXFIFO) >> 4);
}

// Get pipe number for the payload available for reading from RX FIFO
// return: pipe number or 0x07 if the RX FIFO is empty
uint8_t NRF24_GetRXSource(void) {
	return ((NRF24_ReadReg(NRF24_REG_STATUS) & NRF24_MASK_RX_P_NO) >> 1);
}

uint8_t NRF24_GetRXPayloadWidth(void) {
	uint8_t result;
	NRF24_CSN_Low();
	NRF24_LL_RW(NRF24_CMD_R_RX_PL_WID);
	result = NRF24_LL_RW(0x00);
	NRF24_CSN_High();
	return result;
}

// Get auto retransmit statistic
// return: value of OBSERVE_TX register which contains two counters encoded in nibbles:
//   high - lost packets count (max value 15, can be reseted by write to RF_CH register)
//   low  - retransmitted packets count (max value 15, reseted when new transmission starts)
uint8_t NRF24_GetRetransmitCounters(void) {
	return (NRF24_ReadReg(NRF24_REG_OBSERVE_TX));
}

// Reset packet lost counter (PLOS_CNT bits in OBSERVER_TX register)
void NRF24_ResetPLOS(void) {
	uint8_t reg;

	// The PLOS counter is reset after write to RF_CH register
	reg = NRF24_ReadReg(NRF24_REG_RF_CH);
	NRF24_WriteReg(NRF24_REG_RF_CH, reg);
}

// Flush the TX FIFO
void NRF24_FlushTX(void) {
	NRF24_WriteReg(NRF24_CMD_FLUSH_TX, NRF24_CMD_NOP);
}

// Flush the RX FIFO
void NRF24_FlushRX(void) {
	NRF24_WriteReg(NRF24_CMD_FLUSH_RX, NRF24_CMD_NOP);
}

// Clear any pending IRQ flags
void NRF24_ClearIRQFlags(void) {
	uint8_t reg;

	// Clear RX_DR, TX_DS and MAX_RT bits of the STATUS register
	reg = NRF24_ReadReg(NRF24_REG_STATUS);
	reg |= NRF24_MASK_STATUS_IRQ;
	NRF24_WriteReg(NRF24_REG_STATUS, reg);
}

uint8_t NRF24_Transmit() {
	uint8_t status;
	NRF24_CE_High(); // assert CE pin (transmission starts)
	while (1) {
		status = NRF24_GetStatus();
		if (status & (NRF24_FLAG_TX_DS | NRF24_FLAG_MAX_RT)) {
			// transmission ended, exit loop
			break;
		}
	}
	NRF24_CE_Low(); // de-assert CE pin (nRF24 goes to StandBy-I mode)
	NRF24_ClearIRQFlags();
	if (status & NRF24_FLAG_MAX_RT) {
		return 1;
	}
	if (status & NRF24_FLAG_TX_DS) {
			return 0;
	}
	return 2;
}

void NRF24_StartReceive() {
	NRF24_CE_High();
}
void NRF24_StopReceive() {
	NRF24_CE_Low();
}

// Write TX payload
// input:
//   pBuf - pointer to the buffer with payload data
//   length - payload length in bytes
void NRF24_WritePayload(uint8_t *pBuf, uint8_t length) {
	NRF24_WriteMBReg(NRF24_CMD_W_TX_PAYLOAD, pBuf, length);
}

// Read top level payload available in the RX FIFO
// input:
//   pBuf - pointer to the buffer to store a payload data
//   length - pointer to variable to store a payload length
// return: one of NRF24_RX_xx values
//   NRF24_RX_PIPEX - packet has been received from the pipe number X
//   NRF24_RX_EMPTY - the RX FIFO is empty
NRF24_RXResult NRF24_ReadPayload(uint8_t *pBuf, uint8_t *length) {
	uint8_t pipe;

	// Extract a payload pipe number from the STATUS register
	pipe = (NRF24_ReadReg(NRF24_REG_STATUS) & NRF24_MASK_RX_P_NO) >> 1;

	// RX FIFO empty?
	if (pipe < 6U) {
		// Get payload length
		*length = NRF24_ReadReg(NRF24_REG_RX_PW_P0 + pipe);

		// Read a payload from the RX FIFO
		if (*length) {
			NRF24_ReadMBReg(NRF24_CMD_R_RX_PAYLOAD, pBuf, *length);
		}

		return ((NRF24_RXResult)pipe);
	}

	// The RX FIFO is empty
	*length = 0U;

	return NRF24_RX_EMPTY;
}

NRF24_RXResult NRF24_ReadDynPayload(uint8_t *pBuf, uint8_t *length) {
	uint8_t pipe;

	// Extract a payload pipe number from the STATUS register
	pipe = (NRF24_ReadReg(NRF24_REG_STATUS) & NRF24_MASK_RX_P_NO) >> 1;

	// RX FIFO empty?
	if (pipe < 6U) {
		// Get payload length
		*length = NRF24_GetRXPayloadWidth();

		// Read a payload from the RX FIFO
		if (*length) {
			NRF24_ReadMBReg(NRF24_CMD_R_RX_PAYLOAD, pBuf, *length);
		}

		return ((NRF24_RXResult)pipe);
	}

	// The RX FIFO is empty
	*length = 0U;

	return NRF24_RX_EMPTY;
}

// Print NRF24L01+ current configuration (for debug purposes)
void NRF24_DumpConfig(NRF24_printf _printf) {

	uint8_t i, j;
	uint8_t aw;
	uint8_t buf[5];

	// Dump nRF24L01+ configuration

	// CONFIG
	i = NRF24_ReadReg(NRF24_REG_CONFIG);
	_printf("[0x%02X][0x%02X] MASK:%03b CRC:%02b PWR:%s MODE:P%s\r\n",
			NRF24_REG_CONFIG,
			i,
			i >> 4,
			(i & 0x0c) >> 2,
			(i & 0x02) ? "ON" : "OFF",
			(i & 0x01) ? "RX" : "TX"
		);

	// EN_AA
	i = NRF24_ReadReg(NRF24_REG_EN_AA);
	_printf("[0x%02X][0x%02X] ENAA: ", NRF24_REG_EN_AA, i);
	for (j = 0; j < 6; j++) {
		_printf("[P%1u%s]%s",
				j,
				(i & (1 << j)) ? "+" : "-",
				(j == 5) ? "\r\n" : " "
			);
	}

	// EN_RXADDR
	i = NRF24_ReadReg(NRF24_REG_EN_RXADDR);
	_printf("[0x%02X][0x%02X] EN_RXADDR: ", NRF24_REG_EN_RXADDR, i);
	for (j = 0; j < 6; j++) {
		_printf("[P%1u%s]%s", j,
				(i & (1 << j)) ? "+" : "-",
				(j == 5) ? "\r\n" : " "
			);
	}

	// SETUP_AW
	i = NRF24_ReadReg(NRF24_REG_SETUP_AW);
	aw = NRF24_GetAddrWidth();
	_printf("[0x%02X][0x%02X] Address: %u Bytes\r\n",
			NRF24_REG_SETUP_AW,
			i,
			aw
		);

	// SETUP_RETR
	i = NRF24_ReadReg(NRF24_REG_SETUP_RETR);
	_printf("[0x%02X][0x%02X] ARD=%04b ARC=%04b (retr.delay=%uus, count=%u)\r\n",
			NRF24_REG_SETUP_RETR,
			i,
			i >> 4,
			i & 0x0F,
			((i >> 4) * 250) + 250,
			i & 0x0F
		);

	// RF_CH
	i = NRF24_ReadReg(NRF24_REG_RF_CH);
	_printf("[0x%02X][0x%02X] RF channel:%u (%.3uGHz)\r\n",
			NRF24_REG_RF_CH,
			i,
			i,
			2400 + i
		);

	// RF_SETUP
	i = NRF24_ReadReg(NRF24_REG_RF_SETUP);
	_printf("[0x%02X][0x%02X] CONT_WAVE:%s PLL_LOCK:%s DataRate=",
			NRF24_REG_RF_SETUP,
			i,
			(i & 0x80) ? "ON" : "OFF",
			(i & 0x80) ? "ON" : "OFF"
		);
	switch ((i & 0x28) >> 3) {
		case 0x00:
			_printf("1M");
			break;
		case 0x01:
			_printf("2M");
			break;
		case 0x04:
			_printf("250k");
			break;
		default:
			_printf("???");
			break;
	}
	_printf("pbs RF_PWR=");
	switch ((i & 0x06) >> 1) {
		case 0x00:
			_printf("-18");
			break;
		case 0x01:
			_printf("-12");
			break;
		case 0x02:
			_printf("-6");
			break;
		case 0x03:
			_printf("0");
			break;
		default:
			_printf("???");
			break;
	}
	_printf("dBm\r\n");

	// STATUS
	i = NRF24_ReadReg(NRF24_REG_STATUS);
	_printf("[0x%02X][0x%02X] IRQ:%03b RX_PIPE:%u TX_FULL:%s\r\n",
			NRF24_REG_STATUS,
			i,
			(i & 0x70) >> 4,
			(i & 0x0E) >> 1,
			(i & 0x01) ? "YES" : "NO"
		);
	// OBSERVE_TX
	i = NRF24_ReadReg(NRF24_REG_OBSERVE_TX);
	_printf("[0x%02X][0x%02X] PLOS_CNT=%u ARC_CNT=%u\r\n",
			NRF24_REG_OBSERVE_TX,
			i,
			i >> 4,
			i & 0x0F
		);
	// RPD
	i = NRF24_ReadReg(NRF24_REG_RPD);
	_printf("[0x%02X][0x%02X] RPD=%s\r\n",
			NRF24_REG_RPD,
			i,
			(i & 0x01) ? "YES" : "NO"
		);

	// TX_ADDR
	NRF24_ReadMBReg(NRF24_REG_TX_ADDR, buf, aw);
	_printf("[0x%02X] TX_ADDR:    ", NRF24_REG_TX_ADDR);
	for (i = 0; i < aw; i++) { _printf("0x%02x ", buf[i]); }
	_printf("[");
	for (i = 0; i < aw; i++) { _printf("%c", buf[i]); }
	_printf("]\r\n");

	// RX_ADDR_P0
	NRF24_ReadMBReg(NRF24_REG_RX_ADDR_P0, buf, aw);
	_printf("[0x%02X] RX_ADDR_P0: ", NRF24_REG_RX_ADDR_P0);
	for (i = 0; i < aw; i++) { _printf("0x%02x ", buf[i]); }
	_printf("[");
	for (i = 0; i < aw; i++) { _printf("%c", buf[i]); }
	_printf("]\r\n");

	// RX_ADDR_P1
	NRF24_ReadMBReg(NRF24_REG_RX_ADDR_P1, buf, aw);
	_printf("[0x%02X] RX_ADDR_P1: ", NRF24_REG_RX_ADDR_P1);
	for (i = 0; i < aw; i++) { _printf("0x%02x ", buf[i]); }
	_printf("[");
	for (i = 0; i < aw; i++) { _printf("%c", buf[i]); }
	_printf("]\r\n");

	// RX_ADDR_P2
	buf[aw - 1] = NRF24_ReadReg(NRF24_REG_RX_ADDR_P2);
	_printf("[0x%02X] RX_ADDR_P2: ", NRF24_REG_RX_ADDR_P2);
	for (i = 0; i < aw; i++) { _printf("0x%02x ", buf[i]); }
	_printf("[");
	for (i = 0; i < aw; i++) { _printf("%c", buf[i]); }
	_printf("]\r\n");

	// RX_ADDR_P3
	buf[aw - 1] = NRF24_ReadReg(NRF24_REG_RX_ADDR_P3);
	_printf("[0x%02X] RX_ADDR_P3: ", NRF24_REG_RX_ADDR_P3);
	for (i = 0; i < aw; i++) { _printf("0x%02x ", buf[i]); }
	_printf("[");
	for (i = 0; i < aw; i++) { _printf("%c", buf[i]); }
	_printf("]\r\n");

	// RX_ADDR_P4
	buf[aw - 1] = NRF24_ReadReg(NRF24_REG_RX_ADDR_P4);
	_printf("[0x%02X] RX_ADDR_P4: ", NRF24_REG_RX_ADDR_P4);
	for (i = 0; i < aw; i++) { _printf("0x%02x ", buf[i]); }
	_printf("[");
	for (i = 0; i < aw; i++) { _printf("%c", buf[i]); }
	_printf("]\r\n");

	// RX_ADDR_P5
	buf[aw - 1] = NRF24_ReadReg(NRF24_REG_RX_ADDR_P5);
	_printf("[0x%02X] RX_ADDR_P5: ", NRF24_REG_RX_ADDR_P5);
	for (i = 0; i < aw; i++) { _printf("0x%02x ", buf[i]); }
	_printf("[");
	for (i = 0; i < aw; i++) { _printf("%c", buf[i]); }
	_printf("]\r\n");

	// RX_PW_P0
	_printf("[0x%02X] RX_PW_P0=%u\r\n", NRF24_REG_RX_PW_P0, NRF24_ReadReg(NRF24_REG_RX_PW_P0));
	// RX_PW_P1
	_printf("[0x%02X] RX_PW_P1=%u\r\n", NRF24_REG_RX_PW_P1, NRF24_ReadReg(NRF24_REG_RX_PW_P1));
	// RX_PW_P2
	_printf("[0x%02X] RX_PW_P2=%u\r\n", NRF24_REG_RX_PW_P2, NRF24_ReadReg(NRF24_REG_RX_PW_P2));
	// RX_PW_P3
	_printf("[0x%02X] RX_PW_P3=%u\r\n", NRF24_REG_RX_PW_P3, NRF24_ReadReg(NRF24_REG_RX_PW_P3));
	// RX_PW_P4
	_printf("[0x%02X] RX_PW_P4=%u\r\n", NRF24_REG_RX_PW_P4, NRF24_ReadReg(NRF24_REG_RX_PW_P4));
	// RX_PW_P5
	_printf("[0x%02X] RX_PW_P5=%u\r\n", NRF24_REG_RX_PW_P5, NRF24_ReadReg(NRF24_REG_RX_PW_P5));
}
