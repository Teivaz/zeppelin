#include "Dendrite.h"

#include <avr/io.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"
#include "utils.h"

void DendriteInitRx()
{
	char data[2] = {W_REGISTER|CONFIG, 1 << PWR_UP | 1 << EN_CRC | 1 << MASK_RX_DR};
	DendriteWrite(data, 2);
	data[0] = W_REGISTER | RF_SETUP;
	data[1] = 0b00000111;
	DendriteWrite(data, 2);
	data[0] = W_REGISTER | CONFIG;
	data[1] = 1 << PWR_UP | 1 << EN_CRC | 1 << MASK_RX_DR | 1 << PRIM_RX;
	DendriteWrite(data, 2);	
}

void DendriteRead()
{
	char data[33] = {0};
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
		WRITE_REG(USISR, 8);
		WRITE_REG(USIDR, data[a]);
		while(!READ_BIT(USISR, USIOIF));
		CLEAR_BIT(USISR, USIOIF);
	}
	DendriteEnd();
}

void DendriteEnd()
{
	SET_BIT(PORTB, CSN);
}

void DendriteStart()
{
	while(READ_BIT(PINB, CLK));
	CLEAR_BIT(PORTB, CSN);
}

void DendriteInterrupt()
{
	DendriteRead();
	DendriteFinishReading();
}
