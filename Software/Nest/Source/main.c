#include "main.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "nRF24L01.h"
#include "config.h"
#include "utils.h"

#include "StreamBuffer.h"
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
		;
    }
}

void Configure()
{
	InitLetters();

	sei();
}

void CreateSpiPacket(char letter, signed char dcSpeed, char servo)
{
	s_spiPackage[0] = PRIMARY_LETTER;
	s_spiPackage[1] = letter;
	s_spiPackage[2] = dcSpeed;
	s_spiPackage[3] = servo;
	s_spiPackage[4] = CRC(s_spiPackage, 4);
}
