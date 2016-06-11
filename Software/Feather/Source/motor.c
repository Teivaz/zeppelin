#include "motor.h"
#include <avr/interrupt.h>
#include "config.h"
#include "utils.h"

volatile unsigned char s_motorA = 0;
volatile unsigned char s_motorB = 0;

void Motor_SetSpeedSigned(signed char speed)
{
	cli();
	if (speed == 0)
	{
		s_motorA = 0;
		s_motorB = 0;
	}
	else if(speed > 0)
	{
		s_motorA = speed << 1;
		s_motorB = 0;
	}
	else
	{
		s_motorA = 0;
		s_motorB = (-speed) << 1;
	}
	sei();
}

void Motor_SetSpeedUnsigned(unsigned char speed)
{
	cli();
	if(speed > 127)
	{
		unsigned char newSpeed = speed - 127;
		s_motorA = newSpeed << 1;
		s_motorB = 0;
	}
	else
	{
		unsigned char newSpeed = 127 - speed;
		s_motorA = 0;
		s_motorB = newSpeed << 1;
	}
	sei();
}


void Motor_Tick()
{
	if(s_motorA != 0)
		SET_BIT(PORTB, MOTOR_PIN_A);
	if(s_motorB != 0)
		SET_BIT(PORTB, MOTOR_PIN_B);
	
	WRITE_REG(OCR0A, s_motorA);
	WRITE_REG(OCR0B, s_motorB);
}

void Motor_InitTimer()
{
	WRITE_REG(OCR0A, s_motorA);
	WRITE_REG(OCR0B, s_motorB);
}
