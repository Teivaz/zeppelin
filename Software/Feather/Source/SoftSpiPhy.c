#include "SoftSpiPhy.h"

#ifndef _AVR
	#include "CMSIS/stm32f10x.h"
#else
	#include "types.h"	
#endif

extern void OnSpiWordReceived(char message);
extern void OnSpiWordSent(void);
extern char GetSpiSendWord(void);

char s_spiReadBuffer;
short s_spiReadBufferSize = 0;
char s_spiWriteBuffer;
short s_spiWriteBufferSize = 0;

	// If master - set MOSI pin
	// If slave - set MISO pin
extern void WriteSpiDataBit(bool bit);

	// if slave - read MOSI pin
	// if master - read MISO pin
extern bool ReadSpiDataBit(void);

//We need interrupt on level change on CLK pin
void SpiClkIrq()
{
	bool state; // read CLK pin
#ifndef _SPI_OPTIMIZE
	bool front;
	bool shouldReadNow;

	if(CPOL == 0)	// If changed to high - front
		front = state;
	else
		front = !state;

	if(CPHA == 1)
		shouldReadNow = front;
	else
		shouldReadNow = !front;

	if(shouldReadNow)
		_ReadSpi();
	else
		_WriteSpi();

#else
	if((!SPI_CPHA)^(SPI_CPOL^state))
	{
		_SpiReadBit();
		return;
	}
	_SpiWriteBit();
#endif
}

// Now we need read and write functions
void _SpiReadBit()
{
	if(s_spiReadBufferSize == SPI_WORD_SIZE)
	{
		s_spiReadBuffer = 0;
		s_spiReadBufferSize = 0;
	}
	s_spiReadBuffer = s_spiReadBuffer << 1;
	s_spiReadBuffer += ReadSpiDataBit();
	s_spiReadBufferSize += 1;
	
	if(s_spiReadBufferSize == SPI_WORD_SIZE)
	{
		// Now bit is read completely
		OnSpiWordReceived(s_spiReadBuffer);
	}
}

void _SpiWriteBit()
{
	if(s_spiWriteBufferSize == SPI_WORD_SIZE)
	{
		s_spiWriteBuffer = GetSpiSendWord();
		s_spiWriteBufferSize = 0;
	}
	WriteSpiDataBit(0x1 & s_spiWriteBuffer);

	s_spiWriteBuffer = s_spiWriteBuffer >> 1;
	s_spiWriteBufferSize += 1;
	if(s_spiWriteBufferSize == SPI_WORD_SIZE)
	{
		OnSpiWordSent();
	}
}
