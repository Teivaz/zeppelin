#include "main.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"
#include "SystemConfig.h"

#include "Axon.h"
#include "Dendrite.h"

TStreamBuffer s_stream;

int main(void)
{
	InitializeStream(&s_stream);
	Configure();
	while(1)
    {
	 	if(GetStreamBufferSize(&s_stream) == 0)
		{
			WriteStream(&s_stream, PRIMARY_LETTER);
			WriteStream(&s_stream, '1');
			WriteStream(&s_stream, 2);
			WriteStream(&s_stream, 0);
		}
        UpdateAxonState();
    }
}

void Configure()
{
	//Clock
	uint8_t clkpr = (0 & CLKPS0)|
					(0 & CLKPS1)|
					(0 & CLKPS2)|
					(0 & CLKPS3);
	WRITE_REG(CLKPR, CLKPCE);
	WRITE_REG(CLKPR, clkpr);
	
	SET_BIT(GTCCR, TSM);
	{
		SET_BIT(TCCR0B, CS00); // Set source Fcpu/64
		SET_BIT(TCCR0B, CS01);
			
		SET_BIT(TIMSK, OCIE0A); // Interrupt on compare match A
		SET_BIT(TIMSK, OCIE0B); // Interrupt on compare match B
		//SET_BIT(TIMSK, TOV0); // Interrupt on timer overflow
			
		WRITE_REG(OCR0A, 127);
		WRITE_REG(OCR0B, 254);
	}
	// start timers
	CLEAR_BIT(GTCCR, TSM);
	
	
	SET_BIT(PORTB,	CLK);
	SET_BIT(PORTB,	AXON_MOSI);
	SET_BIT(DDRB,	CLK);
	SET_BIT(DDRB,	AXON_MOSI);
	//SET_BIT(PORTB,	DENDRITE_MOSI);
	//SET_BIT(DDRB,	DENDRITE_MOSI);
	
	
	// Wait for clients to start
	Sleep();
	// Finish configure
	s_axonState = EAxonReady;
	sei();
}

void Sleep()
{
	for(uint8_t a = 0; a < 0x0f; ++a)
	{
		for(uint8_t b = 0; b < 0x0f; ++b)
		{
			//for(uint8_t c = 0; c < 0xff; ++c)
			{
				volatile char f = 0;
			}
		}
	}
}

ISR(TIM0_COMPA_vect)
{
	// Generate CLK fall
	CLEAR_BIT(PORTB, CLK);
	
	// === SPI ===
	// Prepare data to send through
	AxonIncrementBit();
}

ISR(TIM0_COMPB_vect)
{
	// Generate CLK rise
	SET_BIT(PORTB, CLK);
	//WRITE_REG(TCNT0, 0);
	// === SPI ===
	// Read data
}
