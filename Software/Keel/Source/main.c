#include <avr/io.h>
#include <avr/interrupt.h>
#include "nRF24L01.h"

#include "utils.h"

#include "SystemConfig.h"
#include "config.h"

#include "main.h"
#include "Dendrite.h"

static void Configure();
static void Sleep();
#if EMULATE_DENDRITE
	static void EmulateDendrite();
#endif

int main(void)
{
	Configure();
	while(1)
    {
#if EMULATE_DENDRITE
		EmulateDendrite();
		continue;
#endif
		// Monitor IRQ of wireless module
		if(!READ_BIT(PINB, DENDRITE_IRQ))
		{
			Dendrite_Interrupt();
		}
		nop();
    }
}

inline void ConfigureSpi()
{
	/* SPI */
	SET_BIT(USICR, USIWM0); // Three wire USI mode
	
	// SCK. Output. Should be triggered manually
	SET_BIT(PORTB, DENDRITE_SCK);
	SET_BIT(DDRB, DENDRITE_SCK); // Output. Should be triggered manually
	CLEAR_BIT(PORTB, DENDRITE_SCK);
	
	// CSN. Output
	SET_BIT(PORTB, DENDRITE_CSN);
	SET_BIT(DDRB, DENDRITE_CSN);
	//CLEAR_BIT(PORTB, DENDRITE_CSN); Keep in high state
	
	// MOSI. Output
	SET_BIT(PORTB, DENDRITE_MOSI);
	SET_BIT(DDRB, DENDRITE_MOSI);
	CLEAR_BIT(PORTB, DENDRITE_MOSI);
	
	// MISO. Input
	//CLEAR_BIT(DDRB, DENDRITE_MISO); // Default
	
	//Input, interrupt
	//SET_BIT(DDRB, DENDRITE_IRQ); // Default
	
	// CE. Output
	SET_BIT(PORTB, DENDRITE_CE);
	SET_BIT(DDRB, DENDRITE_CE);
	CLEAR_BIT(PORTB, DENDRITE_CE);
}

void Configure()
{	
	// No prescale
	uint8_t clkpr = (0 << CLKPS0)|
					(0 << CLKPS1)|
					(0 << CLKPS2)|
					(0 << CLKPS3);
	WRITE_REG(CLKPR, 1 << CLKPCE);
	WRITE_REG(CLKPR, clkpr);
	// With no prescale Fcpu should be 4 MHz

	ConfigureSpi();
	
	// Wait for clients to start
	Sleep();
	
	// Finish configure
	Dendrite_Init();
#if EMULATE_DENDRITE
	SET_BIT(PORTB, DENDRITE_MISO);
	SET_BIT(DDRB, DENDRITE_MISO);
	CLEAR_BIT(PORTB, DENDRITE_MISO);
#endif
}

void Sleep()
{
	for(uint8_t a = 0; a < 0x0f; ++a)
	{
		for(uint8_t b = 0; b < 0x0f; ++b)
		{
			//for(uint8_t c = 0; c < 0xff; ++c)
			{
				nop();
			}
		}
	}
}

#if EMULATE_DENDRITE
static uint8_t s_dendrite0 = 0;
static int8_t s_dendrite1 = 0;


void EmulateSendByte(unsigned char data)
{
	CLEAR_BIT(PORTB, DENDRITE_CSN);
	for(int8_t i = 8; i > 0; --i)
	{
		MSB(data) ? SET_BIT(PORTB, DENDRITE_MISO) : CLEAR_BIT(PORTB, DENDRITE_MISO);
		data = data << 1;
		SET_BIT(PORTB, DENDRITE_SCK);
		CLEAR_BIT(PORTB, DENDRITE_SCK);
	}
	SET_BIT(PORTB, DENDRITE_CSN);
}

void EmulateDendrite()
{
	char package[] = {'Z', '1', s_dendrite0, s_dendrite1, 0};
	package[4] = HASH4(package);
	EmulateSendByte(package[0]);
	EmulateSendByte(package[1]);
	EmulateSendByte(package[2]);
	EmulateSendByte(package[3]);
	EmulateSendByte(package[4]);
	Sleep();
	s_dendrite0 += 1;
	s_dendrite1 += 1;
}
#endif
