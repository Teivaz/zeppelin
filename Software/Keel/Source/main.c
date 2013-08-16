#include "main.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "nRF24L01.h"
#include "config.h"
#include "types.h"

#include "Axon.h"
#include "Dendrite.h"

char s_buffer[STREAM_BUFFER_SIZE];
char* s_bufferFirst = s_buffer;
char* s_bufferLast = s_buffer;

int main(void)
{
	Configure();
    while(1)
    {
		UpdateDendriteState();
        UpdateAxonState();
    }
}

void Configure()
{
	SET_BIT(PORTB,	CLK);
	SET_BIT(DDRB,	CLK);
	SET_BIT(PORTB,	AXON_MOSI);
	SET_BIT(DDRB,	AXON_MOSI);
	SET_BIT(PORTB,	DENDRITE_MOSI);
	SET_BIT(DDRB,	DENDRITE_MOSI);
	
	
	// Wait for clients to start
	Sleep();
	// Finish configure
	s_axonState = EAxonReady;
	sei();
}

//ISR()
//{
	//
//}

void Sleep()
{
	;
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
	
	// === SPI ===
	// Read data
}
