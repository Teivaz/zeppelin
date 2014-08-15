#include "package.h"

void Pack0(char b);
void Pack1(char b);
void Pack2(char b);
void Pack3(char b);
void Pack4(char b);

char s_packageValid = 0;
char s_packageLength = 0;
void(*s_packagePtr)(char) = Pack0;
char s_package[c_packageLength - 1] = {0};
char s_data[2] = {0};
char s_fastCrc[2] = {0};
char s_deadCounter = 0;


void Pack0(char b)
{
	s_fastCrc[0] = b;
	if(b == PRIMARY_LETTER)
	{
		s_package[0] = b;
		s_packagePtr = Pack1;
	}
}

void Pack1(char b)
{
	s_fastCrc[1] = b;
	s_fastCrc[0] = CRC(s_fastCrc, 2);
	if(b == SECONDARY_LETTER)
	{
		s_package[1] = b;
		s_packagePtr = Pack2;
	}
	else
	{
		Package_Reset();
	}
}

void Pack2(char b)
{
	s_deadCounter = 0;
	s_fastCrc[1] = b;
	s_fastCrc[0] = CRC(s_fastCrc, 2);
	s_package[2] = b;
	s_packagePtr = Pack3;
}

void Pack3(char b)
{
	s_deadCounter = 0;
	s_fastCrc[1] = b;
	s_fastCrc[0] = CRC(s_fastCrc, 2);
	s_package[3] = b;
	s_packagePtr = Pack4;
}

void Pack4(char b)
{
	if(s_fastCrc[0] == b)
	{
		// CRC match
		s_deadCounter = 0;
		s_packageValid = 1;
		s_data[0] = s_package[2];
		s_data[1] = s_package[3];
	}
	s_packagePtr = Pack0;
}

void Package_AddByte(char b)
{
	if(s_packageValid)
	{
		Package_Reset();
	}
	s_packagePtr(b);
}

void Package_Reset()
{
	s_packageValid = 0;
	s_packagePtr = Pack0;
}

char Package_IsValid()
{
	return s_packageValid;
}

char Package_GetDataByte(uint8_t num)
{
	return s_data[num];
}

void Package_Ping()
{
	++s_deadCounter;
	if(s_deadCounter > c_maxDeadTicks)
	{
		Package_Reset();
		s_deadCounter = 0;
	}
}
