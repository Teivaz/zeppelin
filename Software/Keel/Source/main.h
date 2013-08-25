#ifndef _MAIN_H_
#define _MAIN_H_
#include "StreamBuffer.h"

extern TStreamBuffer s_stream;

int main();
void Configure();
void Sleep();

void CreateSpiPacket(char letter, signed char dcSpeed, char servo);

#endif //_MAIN_H_