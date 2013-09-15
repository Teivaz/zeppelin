#include "main.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "nRF24L01.h"
#include "config.h"
#include "utils.h"

#include "StreamBuffer.h"
#include "Axon.h"
#include "Dendrite.h"
#include "SystemConfig.h"

const char sc_secondaryLetters[FEATHER_NUM] =\
{'1', '2', '3', '4'};

char s_stickButtonsState[STICK_NUM] = {1};
char s_modeBtnState = 1;
char s_calBtnState = 1;

char s_stickBuffer[STICK_NUM * 2] = {0};
char s_stickBuffer2[STICK_NUM * 2] = {127};
char s_stickBuffer3[STICK_NUM * 2] = {127};

char sc_stickButtonPorts[STICK_NUM] = STICK_BTN_PORTS;
const char sc_stickButtonPins[STICK_NUM] = STICK_BTN_PINS;
const char sc_stickMux[STICK_NUM * 2] = STICK_ADC_PORTS;

signed char s_speed = 0;
char s_spiPackage[5] = {0};

TEMainMode s_mode;

#ifdef TEST
int main_()
#else
int main()
#endif
{
	Configure();
	
	while(1)
    {
		if(s_mode == EModeBase)
		{
			ReadSticks();
			DendriteReadSticks();
			UpdateDendrite();
			Transmit();
		}
		else if(s_mode == EModeCalStick_Center)
		{
			ReadSticks();
			for(uint8_t a = 0; a < STICK_NUM; ++a)
			{
				s_stickCalibration[a].xCenter = s_stickBuffer[a * 2];
				s_stickCalibration[a].yCenter = s_stickBuffer[a * 2 + 1];
				s_stickBuffer2[a * 2] = s_stickBuffer[a * 2];
				s_stickBuffer2[a * 2 + 1] = s_stickBuffer[a * 2 + 1];
				s_stickBuffer3[a * 2] = s_stickBuffer[a * 2];
				s_stickBuffer3[a * 2 + 1] = s_stickBuffer[a * 2 + 1];
			}
		}
		else if(s_mode == EModeCalStick_MinMax)
		{
			ReadSticks();
			for(uint8_t a = 0; a < STICK_NUM * 2; ++a)
			{
				s_stickBuffer2[a] = min(s_stickBuffer2[a], s_stickBuffer[a]);
				s_stickBuffer3[a] = max(s_stickBuffer3[a], s_stickBuffer[a]);
			}

			for(uint8_t a = 0; a < STICK_NUM; ++a)
			{
				s_stickCalibration[a].xRange = abs(s_stickBuffer3[a * 2] - s_stickBuffer2[a * 2]);
				s_stickCalibration[a].xRange = abs(s_stickBuffer3[a * 2 + 1] - s_stickBuffer2[a * 2 + 1]);
			}
		}
	
		UpdateIndicators();
		_delay_ms(QUERY_PERIOD);
    }
}

void Configure()
{
	ConfigureMcu();
	InitLetters();
	DendriteInit();
	AxonInit();
	ConfigureLeds();
	ConifugeureBtnPullups();
	sei();
	
	ConfigureTx();
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
	AxonStreamWrite(W_TX_PAYLOAD);
	
	// This will fill 20 bytes out of 32 that will be sent.
	for(uint8_t a = 0; a < FEATHER_NUM; ++a)
	{
		CreateSpiPacket(sc_secondaryLetters[a], GetMotor(a), GetServo(a));
		for(uint8_t a = 0; a < 5; ++a)
		{
			AxonStreamWrite(s_spiPackage[a]);
		}
	}

	// Fill the rest with zeros
	for(uint8_t a = 0; a < 12; ++a)
	{
		AxonStreamWrite(0);
	}
	AxonProceed();
}

void ReadButtons()
{
	for(uint8_t a = 0; a < STICK_NUM; ++a)
	{
		// Trigger on button change
		if(ReadStickButton(a) != s_stickButtonsState[a])
		{
			// When changed to pressed
			if(0 == ReadStickButton(a))
			{
				DendriteStickButtonPressed(a);
			}
		}
		s_stickButtonsState[a] = ReadStickButton(a); 
	}
	
	// Trigger on button change
	if(READ_BIT(MODE_BTN_PORT, MODE_BTN_PIN) != s_modeBtnState)
	{
		// When changed to released
		if(1 == READ_BIT(MODE_BTN_PORT, MODE_BTN_PIN))
		{
			SwitchMode();
		}
	}
	s_modeBtnState = READ_BIT(MODE_BTN_PORT, MODE_BTN_PIN);
	
	// If button is released
	if(READ_BIT(CAL_BTN_PORT, CAL_BTN_PIN) == 1)
	{
		// If released after enough time
		if(s_calBtnState >= CAL_BTN_TIME)
		{
			StartStickCal();
		}
		else if(s_calBtnState > 0)
		{
			// If button was just released then advance calibration state
			AdvanceMode();
		}
		s_calBtnState = 0;
	}
	else // If pressed start counting
	{
		++s_calBtnState;
	}
}

char ReadStickButton(unsigned char btn)
{
	return READ_BIT(sc_stickButtonPorts[btn], sc_stickButtonPins[btn]);
}

void ReadSticks()
{
	for(int8_t n = 0; n < sizeof(s_stickBuffer); ++n)
	{
		// Set ADC multiplexer
		char mux = READ_REG(ADMUX);
		mux &= 0b11100000;
		mux |= 0b00000111 & sc_stickMux[n];
		WRITE_REG(ADMUX, mux);
		
		// ADC start conversion
		SET_BIT(ADCSRA, ADSC); 
		
		// Wait for conversion to complete
		while(READ_BIT(ADCSRA, ADIF) == 0);
		CLEAR_BIT(ADCSRA, ADIF);
		
		s_stickBuffer[n] = READ_REG(ADCH);
	}
}

#ifndef TEST
void ReadStickRawData(TEStics num, uint8_t *x, uint8_t *y)
{
	*x = s_stickBuffer[num * 2];
	*y = s_stickBuffer[num * 2 + 1];
}
#endif

void AdvanceMode()
{
	switch(s_mode)
	{
		case EModeBase:
		
		break;
		case EModeCalStick_Center:
			s_mode = EModeCalStick_MinMax;
		break;
		case EModeCalStick_MinMax:
			s_mode = EModeBase;
			SaveStickCalibrationValues();
		break;
		case EModeCalFeather:
			s_mode = EModeBase;
		break;
	}
}

void StartStickCal()
{
	switch(s_mode)
	{
		case EModeBase:
			s_mode = EModeCalStick_Center;
		break;
		
		case EModeCalStick_Center:
		case EModeCalStick_MinMax:
			s_mode = EModeBase;
			LoadStickCalibrationValues();
		break;
		
		case EModeCalFeather:
			s_mode = EModeBase;
		break;
	}
}

void UpdateIndicators()
{
	if(s_mode == EModeBase)
	{
		HasPreset(EFrontLeftServo) ? CLEAR_BIT(STICK_FLS_LED_PORT, STICK_FLS_LED_PIN) : SET_BIT(STICK_FLS_LED_PORT, STICK_FLS_LED_PIN);
		HasPreset(EFrontLeftMotor) ? CLEAR_BIT(STICK_FLM_LED_PORT, STICK_FLM_LED_PIN) : SET_BIT(STICK_FLM_LED_PORT, STICK_FLM_LED_PIN);
		HasPreset(EBackRightServo) ? CLEAR_BIT(STICK_BRS_LED_PORT, STICK_BRS_LED_PIN) : SET_BIT(STICK_BRS_LED_PORT, STICK_BRS_LED_PIN);
		HasPreset(EBackRightMotor) ? CLEAR_BIT(STICK_BRM_LED_PORT, STICK_BRM_LED_PIN) : SET_BIT(STICK_BRM_LED_PORT, STICK_BRM_LED_PIN);
		SET_BIT(CAL_LED_PORT, CAL_LED_PIN);
	}
	else if(s_mode == EModeCalStick_MinMax)
	{
		CLEAR_BIT(CAL_LED_PORT, CAL_LED_PIN);
		CLEAR_BIT(STICK_FLS_LED_PORT, STICK_FLS_LED_PIN);
		CLEAR_BIT(STICK_FLM_LED_PORT, STICK_FLM_LED_PIN);
		SET_BIT(STICK_BRS_LED_PORT, STICK_BRS_LED_PIN);
		SET_BIT(STICK_BRM_LED_PORT, STICK_BRM_LED_PIN);
	}
	else if(s_mode == EModeCalStick_Center)
	{
		CLEAR_BIT(CAL_LED_PORT, CAL_LED_PIN);
		CLEAR_BIT(STICK_FLS_LED_PORT, STICK_FLS_LED_PIN);
		CLEAR_BIT(STICK_FLM_LED_PORT, STICK_FLM_LED_PIN);
		CLEAR_BIT(STICK_BRS_LED_PORT, STICK_BRS_LED_PIN);
		CLEAR_BIT(STICK_BRM_LED_PORT, STICK_BRM_LED_PIN);
	}
	
	if(s_dendriteMode == EFrontBack)
	{
		CLEAR_BIT(CAL_LED_PORT, CAL_LED_PIN);
	}
	else
	{
		SET_BIT(CAL_LED_PORT, CAL_LED_PIN);
	}
}

void ConfigureTx()
{
	AxonWriteRegister(CONFIG, 1 << PWR_UP | 1 << EN_CRC); // Enable
	AxonWriteRegister(RF_SETUP, 0b00000111); // Set data rate 1 MHz
}

void ConfigureLeds()
{
	// Prepare to switch port direction
	SET_BIT(STICK_FLS_LED_PORT, STICK_FLS_LED_PIN);
	SET_BIT(STICK_FLM_LED_PORT, STICK_FLM_LED_PIN);
	SET_BIT(STICK_BRS_LED_PORT, STICK_BRS_LED_PIN);
	SET_BIT(STICK_BRM_LED_PORT, STICK_BRM_LED_PIN);
	SET_BIT(MODE_LED_PORT, MODE_LED_PIN);
	SET_BIT(CAL_LED_PORT, CAL_LED_PIN);
	
	// Pin directions
	SET_BIT( *(&STICK_FLS_LED_PORT - 1), STICK_FLS_LED_PIN);
	SET_BIT( *(&STICK_FLM_LED_PORT - 1), STICK_FLM_LED_PIN);
	SET_BIT( *(&STICK_BRS_LED_PORT - 1), STICK_BRS_LED_PIN);
	SET_BIT( *(&STICK_BRM_LED_PORT - 1), STICK_BRM_LED_PIN);
	SET_BIT( *(&MODE_LED_PORT - 1), MODE_LED_PIN);
	SET_BIT( *(&CAL_LED_PORT - 1), CAL_LED_PIN);
}

void ConifugeureBtnPullups()
{
	SET_BIT(CAL_BTN_PORT, CAL_BTN_PIN);
	SET_BIT(MODE_BTN_PORT, MODE_BTN_PIN);
	SET_BIT(sc_stickButtonPorts[0], sc_stickButtonPins[0]);
	SET_BIT(sc_stickButtonPorts[1], sc_stickButtonPins[1]);
	SET_BIT(sc_stickButtonPorts[2], sc_stickButtonPins[2]);
	SET_BIT(sc_stickButtonPorts[3], sc_stickButtonPins[3]);
}

void ConfigureMcu()
{
	/* ADC */
	// AVCC with external capacitor at AREF pin
	SET_BIT(ADMUX, REFS0);
	CLEAR_BIT(ADMUX, REFS1);
	
	// Set storing format
	SET_BIT(ADMUX, ADLAR);
	
	// Enable ADC
	SET_BIT(ADCSRA, ADEN);
	
	/* SPI */
	SET_BIT(SPCR, MSTR);
	SET_BIT(SPCR, SPR1);
	
	
	SET_BIT(PORTB, PB7); // SCK
	SET_BIT(PORTB, PB5); // MOSI
	SET_BIT(PORTB, CSN);
	SET_BIT(DDRB, PB7); // SCK
	SET_BIT(DDRB, PB5); // MOSI
	SET_BIT(DDRB, CSN);
		
	SET_BIT(SPCR, SPE);
}
