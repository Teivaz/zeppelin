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

const char sc_secondaryLetters[FEATHER_NUM] =\
{'1', '2', '3', '4'};

char s_stickButtonsState[STICK_NUM] = {0};

const char sc_stickButtonPins[STICK_NUM] = STICK_BTN_PINS;
const char sc_stickButtonPorts[STICK_NUM] = STICK_BTN_PORTS;

signed char s_speed = 0;
char s_spiPackage[5] = {0};

#ifdef TEST
int main_()
#else
int main()
#endif
{
	InitializeStream(&s_stream);
	Configure();
	DendriteInit();
	AxonInit();
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

void Transmit()
{
	// This will fill 20 bytes out of 32 that will be sent.
	for(uint8_t a = 0; a < FEATHER_NUM; ++a)
	{
		CreateSpiPacket(sc_secondaryLetters[a], GetMotor(a), GetServo(a));
		for(uint8_t a = 0; a < 5; ++a)
		{
			WriteStream(&s_stream, s_spiPackage[a]);
		}
	}

	// Fill the rest with zeros
	for(uint8_t a = 0; a < 12; ++a)
	{
		WriteStream(&s_stream, 0);
	}
	AxonAdvanceToState(EAxonSending);
}

void ReadButtons()
{
	for(uint8_t a = 0; a < STICK_NUM; ++a)
	{
		if(1 == s_stickButtonsState[a] && 0 == ReadStickButton(a))
		{
			DendriteStickButtonPressed(a);
		}
	}
}

char ReadStickButton(unsigned char btn)
{
	return READ_BIT(sc_stickButtonPorts[btn], sc_stickButtonPins[btn]);
}
