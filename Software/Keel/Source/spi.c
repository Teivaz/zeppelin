#include "spi.h"
#include "config.h"
#include "utils.h"
#include <avr/io.h>


char s_spiCounter = 0;
char s_spiByteIn = 0;


void Spi_IntHToL()
{
}

inline void pushBit(char b)
{
	s_spiByteIn << 1;
	s_spiByteIn |= b;
}

void Spi_IntLToH()
{
	// low to high
	// read MISO
	// increment data counter
	++s_spiCounter;
	pushBit(READ_BIT(PORTD, DENDRITE_MISO) != 0);
		
	// check data counter
	if(s_spiCounter >= 8)
	{
		s_spiCounter = 0;
		onByteSent();
		onByteRecieved(s_spiByteIn);
		s_spiByteIn = 0;
	}
}