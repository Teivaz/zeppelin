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
    }
}

void Configure()
{
	InitLetters();
	//Clock
	uint8_t clkpr = (0 << CLKPS0)|
					(0 << CLKPS1)|
					(1 << CLKPS2)|
					(0 << CLKPS3);
	WRITE_REG(CLKPR, 1 << CLKPCE);
	WRITE_REG(CLKPR, clkpr);
	
	
	SET_BIT(USICR, USIWM0); // Three wire USI mode
	
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

