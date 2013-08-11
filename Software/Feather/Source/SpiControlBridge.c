#include "SpiControlBridge.h"
#include "SoftSpiPhy.h"

#ifdef _AVR
	#include <stdint.h>
#endif

char SpiMakeHead(char reciever, char device)
{
	return (1 << 6) | ( (reciever & D_MASK) << 3) | (device & D_MASK);
}

#ifdef _USE_SPI_TRANSMITTER
void SpiInitTransmitter()
{
	uint8_t i = 0;
	for(; i<PACKAGE_SIZE; ++i)
	{
		s_packageToTransmit[i] = 0;
	}

	SetValue(e_HeadR0D0, SpiMakeHead(0, 0));
	SetValue(e_HeadR0D1, SpiMakeHead(0, 1));
									  
	SetValue(e_HeadR1D0, SpiMakeHead(1, 0));
	SetValue(e_HeadR1D1, SpiMakeHead(1, 1));
									  
	SetValue(e_HeadR2D0, SpiMakeHead(2, 0));
	SetValue(e_HeadR2D1, SpiMakeHead(2, 1));
									  
	SetValue(e_HeadR3D0, SpiMakeHead(3, 0));
	SetValue(e_HeadR3D1, SpiMakeHead(3, 1));
}

void SetValue(char parameter, char value)
{
	if(parameter >= PACKAGE_SIZE)
		return;
	s_packageToTransmit[parameter] = value;
}


void OnSpiWordSent()
{
	if(s_nextTransmitByteIndex == PACKAGE_SIZE)
		s_nextTransmitByteIndex = 0;
	s_currentTransmitByte = s_packageToTransmit[s_nextTransmitByteIndex];
	s_nextTransmitByteIndex += 1;
}

char GetSpiSendWord()
{
	return s_currentTransmitByte;
}

#else
inline void OnSpiWordSent(){};
inline char GetSpiSendWord(){return 0;};
#endif



#ifdef _USE_SPI_RECEIVER
void OnSpiWordReceived(char message)
{
	if(s_recieverStatus == READ_AS_HEADER)
	{
		ReadHeader(message);
		return;
	}
	else if(s_recieverStatus == READ_AS_DATA0)
	{
		s_recieverStatus = READ_AS_HEADER;
		s_speed = message;
		return;
	}
	else if(s_recieverStatus == READ_AS_DATA1)
	{
		s_recieverStatus = READ_AS_HEADER;
		s_position = message;
		return;
	}
	else if(s_recieverStatus == READ_AS_NONE)
	{
		s_recieverStatus = READ_AS_HEADER;
		return;
	}
	return;
}

void ReadHeader(char message)
{
	if(message == SpiMakeHead(RECEIVER_ID, 0))
	{
		s_recieverStatus = READ_AS_DATA0;
	}
	else if(message == SpiMakeHead(RECEIVER_ID, 1))
	{
		s_recieverStatus = READ_AS_DATA1;
	}
	else
	{
		s_recieverStatus = READ_AS_NONE;
	}
}

#else
inline void OnSpiWordReceived(char message) {};

#endif
