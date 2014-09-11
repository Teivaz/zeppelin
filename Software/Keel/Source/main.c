#include "main.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"
#include "utils.h"

#include "Dendrite.h"
#include "SystemConfig.h"

signed char s_speed = 0;
char s_spiPackage[5] = {0};

int main(void)
{
	Configure();
	while(1)
    {
		if(!READ_BIT(PINB, INT))
		{
			DendriteInterrupt();
		}
		asm("nop");
    }
}

void Configure()
{
	//Clock
	
	// No prescale
	uint8_t clkpr = (0 << CLKPS0)|
					(0 << CLKPS1)|
					(0 << CLKPS2)|
					(0 << CLKPS3);
	WRITE_REG(CLKPR, 1 << CLKPCE);
	WRITE_REG(CLKPR, clkpr);
	
	
	// Fuses CKSEL0...CKSEL3 set to 1111 for crystal osc	
	
	
	SET_BIT(USICR, USIWM1); // Two wire USI mode
	
	SET_BIT(PORTB,	CLK);
	SET_BIT(PORTB,	CSN);
	SET_BIT(PORTB,	PB1);
	SET_BIT(DDRB,	CLK);
	SET_BIT(DDRB,	CSN);
	SET_BIT(DDRB,	PB1);
	CLEAR_BIT(PORTB,	CLK);
	
	// Wait for clients to start
	sei();
	Sleep();
	// Finish configure
	DendriteInit();
}

void Sleep()
{
	for(uint8_t a = 0; a < 0x0f; ++a)
	{
		for(uint8_t b = 0; b < 0x0f; ++b)
		{
			//for(uint8_t c = 0; c < 0xff; ++c)
			{
				asm("nop");
			}
		}
	}
}

char s_spiCounter = 0;
char s_spiByteIn = 0;
char s_spiByteOut = 0;
#define SPI_QUEUE_SIZE 1 + 33 + 2
char s_spiQueue[SPI_QUEUE_SIZE];
char* s_spiQueuePtrOut = s_spiQueue;
char* s_spiQueuePtrIn = s_spiQueue;

void SetUpSPI()
{
	// Set clock timer
	// Set ddr for MOSI
	// Set ddr for MISO
	// Set ddr for CE
	// Set ddr for CLK
	// Set ddr for CSN
	// Set ddr for IRQ (INT0)
	
	// Set interrupt on timer clock
	// Set interrupt INT0 on fall
	// Enable clock timer
}

void transmitByte(char data)
{
	*s_spiQueuePtrIn = data;
	++s_spiQueuePtrIn;
}

inline char hasByteToTransmitt()
{
	return s_spiQueuePtrOut > s_spiQueuePtrIn;
}

void onByteRecieved(char data)
{
	
}

void onByteSent()
{
	if(hasByteToTransmitt())
		
}

inline char popByte()
{
	if(hasByteToTransmitt())
	{
		--s_spiQueuePtr;
		return *(s_spiQueuePtr + 1);
	}
	return 0;
}

inline char popBit()
{
	char ret = (s_spiByteOut & 0x80) != 0;
	s_spiByteOut << 1;
	return ret;
}

inline void pushBit(char b)
{
	s_spiByteIn << 1;
	s_spiByteIn |= b;
}

TIMER1_COMPA_vect()
{
	if(READ_BIT(PORTD, CLK))
	{
		// high to low
		// write MOSI
	}
	else
	{
		// low to high
		// read MISO
		// increment data counter
		++s_spiCounter;
		pushBit(READ_REG(PORTD, MISO) != 0);
		
		// check data counter
		if(s_spiCounter >= 8)
		{
			s_spiCounter = 0;
			onByteSent();
			onByteRecieved(s_spiByteIn);
			s_spiByteIn = 0;
		}
	}
}

INT0_vect()
{
	// Read buffer
	// Store data
	// Reset buffer
}