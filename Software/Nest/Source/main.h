#ifndef _MAIN_H_
#define _MAIN_H_

#define F_CPU 8000000UL

int main();
void Configure();
void ConfigureLeds();
void ConifugeureBtnPullups();
void ConfigureTx();

void CreateSpiPacket(char letter, signed char dcSpeed, char servo);
void Transmit();

void ReadButtons();
void ReadSticks();
char GetStikAdcIndex(unsigned char index);

char ReadStickButton(unsigned char btn);

void AdvanceMode();
void StartStickCal();

void UpdateIndicators();
typedef enum
{
	EModeBase = 0,
	EModeCalStick_Center,
	EModeCalStick_MinMax,
	EModeCalFeather
} TEMainMode;

#endif //_MAIN_H_
