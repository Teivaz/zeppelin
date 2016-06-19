#include <avr/io.h>

#include "nRF24L01.h"

#include "config.h"
#include "utils.h"

#include "Dendrite.h"

static void DendriteStart();
static void DendriteEnd();
static char DendriteReadReg(char reg);
static void DendriteWriteReg(char reg, char data);
static void DendriteWrite(char* data, char size);
static void DendriteRead();
static void DendriteFinishReading();
static void DendriteInitRx();
static void FlushRx();
static void FlushTx();
static char DendreiteReadByte();
static void DendriteSetRegBit(char reg, char bit);
static void DendriteClearRegBit(char reg, char bit);

void FlushTx()
{
	char data = FLUSH_TX;
	DendriteWrite(&data, 1);
}

void FlushRx()
{
	char data = FLUSH_RX;
	DendriteWrite(&data, 1);
}

char DendriteReadReg(char reg)
{
	char data[2] = {R_REGISTER | reg, 0xFF};
	DendriteWrite(data, 2);
	return DendreiteReadByte();
}

void DendriteWriteReg(char reg, char data)
{
	char pack[2] = {W_REGISTER | reg, data};
	DendriteWrite(pack, 2);
}

void DendriteSetRegBit(char reg, char bit)
{
	char val = DendriteReadReg(reg);
	SET_BIT(val, bit);
	DendriteWriteReg(reg, val);
}
void DendriteClearRegBit(char reg, char bit)
{
	char val = DendriteReadReg(reg);
	CLEAR_BIT(val, bit);
	DendriteWriteReg(reg, val);
}

void DendriteInitRx()
{
	DendriteWriteReg(CONFIG, (1 << PWR_UP)); // Power up
	DendriteWriteReg(CONFIG, (1 << PWR_UP) | (1 << PRIM_RX)); // Receive mode
	// Interrupt TX_DS enabled by default
	DendriteWriteReg(RX_PW_P0, DENDRITE_PIPE_LENGTH); // Data pipe 0 enabled. 32 bytes
	DendriteWriteReg(RF_SETUP, 0b00000111); // 1 Mbps
	DendriteWriteReg(EN_AA, 0);	// No acknowledge
	SET_BIT(PORTB, DENDRITE_CE); // Activate chip
}

void DendriteRead()
{
	// Will send read request and 32 empty bytes for reading
	// Feathers will automatically receive this data
	char data[DENDRITE_PIPE_LENGTH+1] = {0};
	data[0] = R_RX_PAYLOAD;
	DendriteWrite(data, DENDRITE_PIPE_LENGTH+1);
}

void DendriteFinishReading()
{
	// Reset all interrupt flags
	DendriteWriteReg(STATUS, (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT));
	FlushRx();
	FlushTx();
}

void Dendrite_Init()
{
	DendriteEnd();
	DendriteInitRx();
}

void DendriteWrite( char* data, char size )
{
	DendriteStart();
	const uint8_t a = (1 << USIWM0) | (1 << USITC);
	const uint8_t b = (1 << USIWM0) | (1 << USITC) | (1 << USICLK);
	for(uint8_t i = 0; i < size; ++i)
	{
		WRITE_REG(USIDR, data[i]);
		for(uint8_t j = 8; j > 0; --j)
		{
			WRITE_REG(USICR, a);
			WRITE_REG(USICR, b);
		}
	}
	DendriteEnd();
}

void DendriteEnd()
{
	SET_BIT(PORTB, DENDRITE_CSN);
}

void DendriteStart()
{
	//while(READ_BIT(PINB, CLK));
	CLEAR_BIT(PORTB, DENDRITE_CSN);
}

void Dendrite_Interrupt()
{
	DendriteRead();
	DendriteFinishReading();
}

char DendreiteReadByte()
{
	return READ_REG(USIDR);
}
