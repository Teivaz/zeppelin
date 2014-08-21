#include "package.h"

#define BUFFER_SIZE 33
char si_buffer[BUFFER_SIZE];
char si_bufferLen = 0;
char s_buffer[BUFFER_SIZE];
char s_bufferLen = 0;

char s_pipe0[4];
char s_pipe1[4];
char s_pipe2[4];
char s_pipeState0 = 0;
char s_pipeState1 = 0;
char s_pipeState2 = 0;

char s_payload[2];
char s_hasPayload = 0;

void Package_Init()
{
	s_pipeState0 = 0;
	s_pipeState1 = 0;
	s_pipeState2 = 0;
	s_hasPayload = 0;
	s_bufferLen = 0;
	si_bufferLen = 0;
}

void PackageI_OnReceived(char b)
{
	si_buffer[si_bufferLen] = b;
	++si_bufferLen;
}

char Package_IsDirty()
{
	return si_bufferLen;
}

void Package_Process()
{
	cli();
	memcpy(s_buffer, si_buffer, si_bufferLen);
	s_bufferLen = si_bufferLen;
	si_bufferLen = 0;
	sei();
	
	for(char i = 0; i < s_bufferLen; ++i)
	{
	char data = s_buffer[i];
	
	char advance =	(s_pipeState0 == 0 && data == PRIMARY_LETTER) || 
					(s_pipeState0 == 1 && data == SECONDARY_LETTER) || 
					(s_pipeState0 > 1);
	char skip1 = (s_pipeState0 == 0) && (s_pipeState1 == 0);
	char skip2 = (s_pipeState0 == 0) && (s_pipeState2 == 0);
	if(advance)
	{
		s_pipe0[s_pipeState0] = data;
		++s_pipeState0;
	}
	else
	{
		s_pipeState0 = 0;
	}
	
	if(!skip1)
	{
		advance =	(s_pipeState1 == 0 && data == PRIMARY_LETTER) ||
					(s_pipeState1 == 1 && data == SECONDARY_LETTER) ||
					(s_pipeState1 > 1);
		skip2 = skip2 || (s_pipeState1 == 0);
		if(advance)
		{
			s_pipe1[s_pipeState1] = data;
			++s_pipeState1;
		}
		else
		{
			s_pipeState1 = 0;
		}
	}

	if(!skip2)
	{
		advance =	(s_pipeState2 == 0 && data == PRIMARY_LETTER) ||
					(s_pipeState2 == 1 && data == SECONDARY_LETTER) ||
					(s_pipeState2 > 1);
		if(advance)
		{
			s_pipe2[s_pipeState2] = data;
			++s_pipeState2;
		}
		else
		{
			s_pipeState2 = 0;
		}
	}
	if(s_pipeState0 > 4)
	{
		if(CRC4(s_pipe0) == data)
		{
			Package_Store(s_pipe0);
		}
		else
		{
			s_pipeState0 = 0;
		}
	}

	if(s_pipeState1 > 4)
	{
		if(CRC4(s_pipe1) == data)
		{
			Package_Store(s_pipe1);
		}
		else
		{
			s_pipeState1 = 0;
		}
	}
	
	if(s_pipeState2 > 4)
	{
		if(CRC4(s_pipe2) == data)
		{
			Package_Store(s_pipe2);
		}
		else
		{
			s_pipeState2 = 0;
		}
	}
	}
	
	
}

void Package_Store(char* a)
{
	s_payload[0] = a[2];
	s_payload[1] = a[3];
	s_hasPayload = 1;
}

char Package_GetData(char b)
{
	s_hasPayload = 0;
	return s_payload[b];
};

Package_ResetAllBuffers()
{
	return;
	s_pipeState0 = 0;
	s_pipeState1 = 0;
	s_pipeState2 = 0;
	si_bufferLen = 0;
}

char Package_PayloadDetected()
{
	return s_hasPayload;
}
