#include "../Source/main.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "nRF24L01.h"
#include "../Source/config.h"
#include "utils.h"

#include "../Source/StreamBuffer.h"
#include "../Source/Axon.h"
#include "../Source/Dendrite.h"
#include "SystemConfig.h"


extern int main_();
extern TStreamBuffer s_stream;

uint8_t s_x;
uint8_t s_y;

int main()
{
	
	InitializeStream(&s_stream);
	Configure();
	DendriteInit();
	AxonInit();
	
	s_y = 0;
	s_x = 0;
	
	while(1)
	{
		DendriteReadSticks();
		UpdateDendrite();
		Transmit();
	}
}


void ReadStickRawData(TEStics num, uint8_t *x, uint8_t *y)
{
	*x = s_x;
	*y = s_y;
}
