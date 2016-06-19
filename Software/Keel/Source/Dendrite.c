#include "Dendrite.h"

#include <avr/io.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"
#include "utils.h"

#define PIPE_LENGTH 32

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
	DendriteWriteReg(RX_PW_P0, PIPE_LENGTH); // Data pipe 0 enabled. 32 bytes
	DendriteWriteReg(RF_SETUP, 0b00000111); // 1 Mbps
	DendriteWriteReg(EN_AA, 0);	// No acknowledge
}

void DendriteRead()
{
	// Will send read request and 32 empty bytes for reading
	// Feathers will automatically receive this data
	char data[PIPE_LENGTH+1] = {0};
	data[0] = R_RX_PAYLOAD;
	DendriteWrite(data, PIPE_LENGTH+1);
}

void DendriteFinishReading()
{
	// Reset all interrupt flags
	DendriteWriteReg(STATUS, (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT));
	FlushRx();
	FlushTx();
}

void DendriteInit()
{
	DendriteEnd();
	DendriteInitRx();
}

void DendriteWrite( char* data, char size )
{
	DendriteStart();
	for(uint8_t a = 0; a < size; ++a)
	{
		WRITE_REG(USISR, 0);
		WRITE_REG(USIDR, data[a]);
		for(uint8_t b = 0; b < 8; ++b)
		{
			{ // Front
				SET_BIT(PORTB, DENDRITE_SCK);
			}
			{ // Rear
				CLEAR_BIT(PORTB, DENDRITE_SCK);
				SET_BIT(USICR, USICLK);
			}
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

void DendriteInterrupt()
{
	DendriteRead();
	DendriteFinishReading();
}

void DendriteToggle(char front)
{

}

char DendreiteReadByte()
{
	return READ_REG(USIDR);
}
