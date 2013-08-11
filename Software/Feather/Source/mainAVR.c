#include "mainAVR.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code 
    }
}

void Init()
{
	// Default frequency is 9.6 MHz divided by 8
	// System frequency = 1.2 MHz
	
	/* === Configure prescale here ===	* /
	uint8_t clkpr = (0 & CLKPS0)|
					(0 & CLKPS1)|
					(0 & CLKPS2)|
					(0 & CLKPS3);
	WRITE_REG(CLKPR, CLKPCE);
	WRITE_REG(CLKPR, clkpr);
	/**/
	
	
	/* === Configure timer here === */
	SET_BIT(TCCR0B, CS02);
	// divide by 256
	// 4.69 kHz min = 0.21 ms, max = 54ms
	
	SET_BIT(TIMSK0, OCIE0A);
	SET_BIT(TIMSK0, OCIE0B);
	WRITE_REG(OCR0A, 7);	// 1.5 ms
	WRITE_REG(OCR0B, 95);	// 20 ms
	
	SET_BIT(DDRB, DDB4);
	
	sei();
}

ISR(TIM0_COMPA_vect)
{	// High ends here
	CLEAR_BIT(PORTB, PORTB4);
}

ISR(TIM0_COMPB_vect)
{	// Low ends here
	SET_BIT(PORTB, PORTB4);
	WRITE_REG(TCNT0, 0);
}
