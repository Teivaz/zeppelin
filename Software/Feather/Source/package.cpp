#include "package.h"

char s_packageValid = 0;
char s_packageLength = 0;
char s_package[c_packageLength] = {0};

void Package_AddByte(char byte)
{
	if(s_packageLength >= c_packageLength)
	{
		Package_Reset();
	}
	
	if(s_packageLength == 0)
	{
		// Here should be primary letter
		if(byte == PRIMARY_LETTER)
		{
			s_package[s_packageLength] = byte;
			s_packageLength += 1;
		}
		else
		{
			Package_Reset();
		}
	}
	else if(s_packageLength == 1)
	{
		// Here should be secondary letter
		if(byte == SECONDARY_LETTER)
		{
			s_package[s_packageLength] = byte;
			s_packageLength += 1;
		}
		else
		{
			Package_Reset();
		}
	}
	else if(s_packageLength == 2)
	{
		// First data byte
		s_package[s_packageLength] = byte;
		s_packageLength += 1;
	}
	else if(s_packageLength == 3)
	{
		// Second data byte
		s_package[s_packageLength] = byte;
		s_packageLength += 1;
	}
	else if(s_packageLength == 4)
	{
		// CRC
		if(CRC(s_package, 4) == byte)
		{
			s_packageValid = 1;
		}
		else
		{
			Package_Reset();
		}
	}
}

void Package_Reset()
{
	s_packageValid = 0;
	s_packageLength = 0;
}

char Package_IsValid()
{
	return s_packageValid;
}

char Package_GetDataByte(uint8_t num)
{
	return s_package[2 + num];
}
