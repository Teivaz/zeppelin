#include "package.h"
#include <avr/interrupt.h>
#include "SystemConfig.h"
#include "utils.h"
#include "buffer.h"

Buffer S_buffer;
char* S_payload;

void Package_Init()
{
	S_payload = 0;
	Buffer_Init(&S_buffer);
}

char Package_IsDirty()
{
	return Buffer_HasData(&S_buffer);
}

void PackageI_OnReceived(char b)
{
	Buffer_Push(&S_buffer, b);
}

void Package_Process()
{
	while(Buffer_HasData(&S_buffer))
	{
		S_payload = Buffer_Read(&S_buffer);
		if(S_payload)
		{
			break;
			Buffer_MarkRead(&S_buffer);
		}
	}
}

char* Package_GetPayload()
{
	return S_payload;
};

void Package_ClearState()
{
	S_payload = 0;
}
