#ifndef _MAIN_H_
#define _MAIN_H_

int main();
void Configure();

void CreateSpiPacket(char letter, signed char dcSpeed, char servo);
void Transmit();

void ReadButtons();
void ReadSticks();

char ReadStickButton(char btn);

#endif //_MAIN_H_