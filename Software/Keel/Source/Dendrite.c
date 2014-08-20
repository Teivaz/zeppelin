#include "Dendrite.h"

#include <avr/io.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"
#include "utils.h"

char s_dendriteSending = 0;

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
	DendriteWriteReg(CONFIG, (1 << PWR_UP) | (0 << PRIM_RX));
	DendriteWriteReg(CONFIG, (1 << PWR_UP) | (1 << PRIM_RX));
	DendriteWriteReg(RX_PW_P0, 32); // Data pipe 0
	DendriteWriteReg(RF_SETUP, 0b00000111); // 1 Mbps
	DendriteWriteReg(EN_AA, 0);	// No acknoledge
}

void DendriteRead()
{
	char data[33] = {0};
	data[0] = R_RX_PAYLOAD;
	DendriteWrite(data, 33);
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
			DendriteToggle(1);
			DendriteToggle(0);
		}
	}
	DendriteEnd();
}

void DendriteEnd()
{
	SET_BIT(PORTB, CSN);
	s_dendriteSending = 0;
}

void DendriteStart()
{
	s_dendriteSending = 1;
	//while(READ_BIT(PINB, CLK));
	CLEAR_BIT(PORTB, CSN);
}

void DendriteInterrupt()
{
	DendriteRead();
	DendriteFinishReading();
}

void DendriteToggle(char front)
{
	//if(s_dendriteSending)
	{
		if(front == 0)
		{
			CLEAR_BIT(PORTB, CLK);
			SET_BIT(USICR, USICLK);
		}
		else
		{
			SET_BIT(PORTB, CLK);
		}
	}
}

char DendreiteReadByte()
{
	return READ_REG(USIDR);
}
