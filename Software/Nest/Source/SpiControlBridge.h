#ifndef _SPICONTROLBRIDGE_H_
#define _SPICONTROLBRIDGE_H_

#include "CMSIS/STM32F10x.h"

#define _USE_SPI_RECIEVER
#define _USE_SPI_TRANSMITTER

// Transmitter
void OnSpiWordSent(void);
char GetSpiSendWord(void);

enum
{
	e_HeadR0D0,	e_DataR0D0,
	e_HeadR0D1,	e_DataR0D1,
	e_HeadR1D0,	e_DataR1D0,
	e_HeadR1D1,	e_DataR1D1,
	e_HeadR2D0,	e_DataR2D0,
	e_HeadR2D1,	e_DataR2D1,
	e_HeadR3D0,	e_DataR3D0,
	e_HeadR3D1,	e_DataR3D1,
	PACKAGE_SIZE
};

#define D_MASK 7	// 0111
char SpiMakeHead(char reciever, char device);

#ifdef _USE_SPI_TRANSMITTER
void SpiInitTransmitter(void);
void SetValue(char parameter, char value);

char s_packageToTransmit[PACKAGE_SIZE];
char s_nextTransmitByteIndex = 0;
char s_currentTransmitByte = 0;
#endif

// Receiever
void OnSpiWordReceived(char message);

#ifdef _USE_SPI_RECIEVER

#define RECIEVER_ID 1

extern char s_speed = 0;
extern char s_position = 0;

void ReadHeader(char message);
//void SaveData(char data);

char GetSpeed(void);	
char GetRotation(void);

enum
{
	READ_AS_HEADER = 0,
	READ_AS_DATA0,
	READ_AS_DATA1,
	READ_AS_NONE,
};

char s_recieverStatus = 0;

#endif
#endif //_SPICONTROLBRIDGE_H_
