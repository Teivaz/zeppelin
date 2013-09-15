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
	//DendriteWriteReg(CONFIG, (1 << PWR_UP) | (1 << EN_CRC) | (1 << MASK_RX_DR) | (1 << MASK_TX_DS) | (1 << MASK_MAX_RT) | (1 << PRIM_RX));
	DendriteSetRegBit(CONFIG, PWR_UP);
	DendriteSetRegBit(CONFIG, PRIM_RX);
	DendriteSetRegBit(CONFIG, MASK_TX_DS);
	
	
	DendriteWriteReg(RF_SETUP, 0b00000111);
	
	DendriteWriteReg(EN_AA, 0);
	DendriteWriteReg(RX_PW_P0, 32);
	DendriteWriteReg(EN_RXADDR, 1 << ERX_P0);
	char addr[6] = {W_REGISTER | RX_ADDR_P0, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	DendriteWrite(addr, 6);
	
	DendriteWriteReg(SETUP_RETR, 0);
	
	//DendriteWriteReg(CONFIG, (1 << PWR_UP) | (1 << EN_CRC) | (1 << MASK_RX_DR) | (1 << MASK_TX_DS) | (1 << MASK_MAX_RT) | (1 << PRIM_RX));
	
	FlushRx();
	FlushTx();
}

void DendriteRead()
{
	char data[33] = {0xff};
	data[0] = R_RX_PAYLOAD;
	DendriteWrite(data, 33);
}

void DendriteFinishReading()
{
	char data[2] = {W_REGISTER | STATUS, 0};
	DendriteWrite(data, 2);
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
		//while(!READ_BIT(USISR, USIOIF));
		//CLEAR_BIT(USISR, USIOIF);
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
			asm("nop");
			CLEAR_BIT(PORTB, CLK);
			SET_BIT(USICR, USICLK);
		}
		else
		{
			asm("nop");
			SET_BIT(PORTB, CLK);
		}
	}
}

char DendreiteReadByte()
{
	return READ_REG(USIDR);
}
