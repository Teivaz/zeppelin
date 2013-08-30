#include "main.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"
#include "utils.h"

#include "Axon.h"
#include "Dendrite.h"
#include "SystemConfig.h"

TStreamBuffer s_stream;

signed char s_speed = 0;
char s_spiPackage[5] = {0};

int main(void)
{
	InitializeStream(&s_stream);
	Configure();
	while(1)
    {
		if(!READ_BIT(PORTB, INT))
		{
			OnDendriteInterrupt();
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
	OnDendriteSpiReady();
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

void CreateSpiPacket(char letter, signed char dcSpeed, char servo)
{
	s_spiPackage[0] = PRIMARY_LETTER;
	s_spiPackage[1] = letter;
	s_spiPackage[2] = dcSpeed;
	s_spiPackage[3] = servo;
	s_spiPackage[4] = CRC(s_spiPackage, 4);
}

ISR(TIM0_COMPA_vect)
{
	// Generate CLK fall
	CLEAR_BIT(PORTB, CLK);
	//SET_BIT(USICR, USITC);
	
	// === SPI ===
	// Prepare data to send through
	SET_BIT(USICR, USICLK);
	
	char counter = READ_REG(USISR) & (1<<USICNT0 | 1<<USICNT1 | 1<<USICNT2 | 1<<USICNT3);
	if(counter > 7)
	{
		CLEAR_BIT(USISR, USICNT0);
		CLEAR_BIT(USISR, USICNT1);
		CLEAR_BIT(USISR, USICNT2);
		CLEAR_BIT(USISR, USICNT3);
		OnDendriteSpiReady();
	}
	//AxonIncrementBit();
}

ISR(TIM0_COMPB_vect)
{
	// Generate CLK rise
	SET_BIT(PORTB, CLK);
	//SET_BIT(USICR, USITC);
	
	//WRITE_REG(TCNT0, 0);
	// === SPI ===
	// Read data
}
