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
		if(!READ_BIT(PORTB, INT))
		{
			DendriteInterrupt();
		}
    }
}

void Configure()
{
	InitLetters();
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
		//SET_BIT(TCCR0B, CS01);
			
		SET_BIT(TIMSK, OCIE0A); // Interrupt on compare match A
		SET_BIT(TIMSK, OCIE0B); // Interrupt on compare match B
		//SET_BIT(TIMSK, TOV0); // Interrupt on timer overflow
			
		WRITE_REG(OCR0A, 128);
		WRITE_REG(OCR0B, 255);
	}
	// start timers
	CLEAR_BIT(GTCCR, TSM);
	
	SET_BIT(USICR, USIWM0); // Three wire USI mode
	
	SET_BIT(PORTB,	CLK);
	SET_BIT(DDRB,	CLK);
	SET_BIT(PORTB,	CSN);
	SET_BIT(DDRB,	CSN);
	SET_BIT(DDRB,	PB1);
	SET_BIT(PORTB,	PB1);
	
	// Wait for clients to start
	Sleep();
	// Finish configure
	sei();
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

ISR(TIM0_COMPA_vect)
{
	// Generate CLK fall
	CLEAR_BIT(PORTB, CLK);
	
	// === SPI ===
	// Prepare data to send through
	SET_BIT(USICR, USICLK);
}

ISR(TIM0_COMPB_vect)
{
	// Generate CLK rise
	SET_BIT(PORTB, CLK);
	//SET_BIT(USICR, USITC);
	
	// === SPI ===
	// Read data
}
