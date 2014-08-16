#include "package.h"

char checkBuffer(char* buf);
inline void storeBuffer(char* in, char* out);

char s_pack1[5] = {0};
char s_id1 = 0;
char s_pack2[5] = {0};
char s_id2 = 1;
char s_pack3[5] = {0};
char s_id3 = 2;
char s_pack4[5] = {0};
char s_id4 = 3;
char s_pack5[5] = {0};
char s_id5 = 4;

char s_tmp[5] = {0};
char s_tmpIdx = 0;
char si_tmpIdx = 0;
char si_tmp[5] = {0};

char s_data[2] = {0};
char s_payloadDetected = 0;

void Package_Init()
{
	
}

void PackageI_OnReceived(char b)
{
	si_tmp[si_tmpIdx++] = b;
}

char Package_IsDirty()
{
	return si_tmpIdx;
}

void Package_Process()
{
	cli();
	if(si_tmpIdx == 0)
	{
		sei();
		return;
	}
	memcpy(s_tmp, si_tmp, si_tmpIdx);
	s_tmpIdx = si_tmpIdx;
	si_tmpIdx = 0;
	sei();
	
	while(--s_tmpIdx > 0)
	{
		char data = s_tmp[s_tmpIdx - 1];
		s_pack1[s_id1] = data;
		s_id1 = (++s_id1) % 5;
		s_pack2[s_id2] = data;
		s_id2 = (++s_id2) % 5;
		s_pack3[s_id3] = data;
		s_id3 = (++s_id3) % 5;
		s_pack4[s_id4] = data;
		s_id4 = (++s_id4) % 5;
		s_pack5[s_id5] = data;
		s_id5 = (++s_id5) % 5;
	}
	
	if(1 == checkBuffer(s_pack1))
	{
		storeBuffer(s_pack1, s_data);
	}
	if(1 == checkBuffer(s_pack2))
	{
		storeBuffer(s_pack2, s_data);
	}
	if(1 == checkBuffer(s_pack3))
	{
		storeBuffer(s_pack3, s_data);
	}
	if(1 == checkBuffer(s_pack4))
	{
		storeBuffer(s_pack4, s_data);
	}
	if(1 == checkBuffer(s_pack5))
	{
		storeBuffer(s_pack5, s_data);
	}
}

char Package_PayloadDetected()
{
	return s_payloadDetected;
}

char Package_GetData(char b)
{
	s_payloadDetected = 0;
	return s_data[b];
}

char checkBuffer(char* buf)
{
	if(buf[0] != PRIMARY_LETTER)
		return 0;
	if(buf[1] != SECONDARY_LETTER)
		return 0;		
	char crc = CRC(buf, 4);
	if(buf[4] != crc)
		return 0;
	return 1;
}

inline void storeBuffer(char* in, char* out)
{
	out[0] = in[2];
	out[1] = in[3];
	s_payloadDetected = 1;
}
