#ifndef SERVO_H_
#define SERVO_H_
#include "config.h"

#define SERVO_MIN_IMPULSE	100	// This value should be around 0.5 ms. 1 equals to 0.005 ms
#define SERVO_PAUSE			80	// This value should be around 20 ms. 1 equals 0.25 ms

void Servo_Init();

// Call when timers are disabled (probably)
inline void Servo_InitTimer()
{
	WRITE_REG(OCR1A, SERVO_PAUSE);
}

// Tick watchdog
inline void Servo_WDTick()
{
	extern unsigned char s_spiWatchdog;
	++s_spiWatchdog;
}

void(*Servo_AdvanceState)();

void Servo_SetPosition(unsigned char position);

#endif /* SERVO_H_ */