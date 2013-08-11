#include "mainAVR.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char position = 40;	//7...40

int main(void)
{
	Init();
    while(1)
    {
		;
    }
}

void Init()
{
	// Default frequency is 9.6 MHz divided by 8
	// System frequency = 1.2 MHz
	
	/* === Configure prescale here ===	*/
	uint8_t clkpr = (0 & CLKPS0)|
					(0 & CLKPS1)|
					(0 & CLKPS2)|
					(0 & CLKPS3);
	WRITE_REG(CLKPR, CLKPCE);
	WRITE_REG(CLKPR, clkpr);
	
	
	
	/* === Configure timer here === */
	SET_BIT(TCCR0B, CS00);
	SET_BIT(TCCR0B, CS01);
	// divide by 256
	// 4.69 kHz min = 0.21 ms, max = 54ms
	
	SET_BIT(TIMSK0, OCIE0A);
	
	SET_BIT(PORTB, PB3);
	SET_BIT(PORTB, PB4);
	SET_BIT(DDRB, DDB4);
	SET_BIT(DDRB, DDB3);
	
	sei();
}

ISR(TIM0_COMPA_vect)
{	// High ends here
	
	char prevLevel = READ_BIT(PORTB, PB4);
	if(prevLevel == 0)
	{
		// High Impulse
		
		// Set timer prescale
		SET_BIT(TCCR0B, CS00);
		SET_BIT(TCCR0B, CS01);
		CLEAR_BIT(TCCR0B, CS02);
		WRITE_REG(TCNT0, 0);
				
		SET_BIT(PORTB, PB4);
		WRITE_REG(OCR0A, position);
	}
	else
	{
		// Set timer prescale
		CLEAR_BIT(TCCR0B, CS00);
		CLEAR_BIT(TCCR0B, CS01);
		SET_BIT(TCCR0B, CS02);
		WRITE_REG(TCNT0, 0);
		
		CLEAR_BIT(PORTB, PB4);
		WRITE_REG(OCR0A, 64);
	}
}
