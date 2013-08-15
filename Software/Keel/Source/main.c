#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code 
    }
}

void UpdateAxonStatus(unsigned char state)
{
	switch (state)
	{
	case EAxonHalt:
	// This status should remove configurer
		break;
		
	case EAxonReadyToSend:
		// Check if has something to send
		if(s_dendriteState == EDendriteReady)
		{
			AxonSend(s_buffer);
		}
		break;
		
	case EAxonSending:
	
		
		break;
	}
}

ISR(TIM0_COMPA_vect)
{
	// Generate SCK fall
	
	// === SPI ===
	// Prepare data to send through
}

ISR(TIM0_COMPB_vect)
{
	// Generate SCK rise
	
	// === SPI ===
	// Read data
}
