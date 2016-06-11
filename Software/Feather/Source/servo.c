#include "servo.h"
#include "config.h"
#include "utils.h"
#include <avr/interrupt.h>

unsigned char s_spiWatchdog = 1;
unsigned char s_spiWatchdogPrev = 0;

typedef enum
{
	EServoFirst,
	EServoSecond,
	EServoPause
}	TServoState;

volatile uint8_t s_servo = 63;	// 1 equals 0.0078 ms (78 us) + 0.5 ms

void ServoPause();
void ServoFirst();
void ServoSecond();

volatile char s_servoState = EServoPause;

void Servo_Init()
{
	Servo_AdvanceState = ServoPause;
}

void Servo_SetPosition(unsigned char position)
{
	char tmp = max(2, position);
	tmp = min(200, tmp);
	cli();
	// clamp 2...200
	s_servo = tmp;
	sei();
}

void ServoPause()
{
	SET_BIT(PORTB, SERVO_PIN);
	// PCK/4
	uint8_t tmp = TCCR1;
	CLEAR_BIT(tmp, CS13);
	CLEAR_BIT(tmp, CS12);
	SET_BIT(tmp, CS11);
	SET_BIT(tmp, CS10);
	TCCR1 = tmp;
	
	CLEAR_REG(TCNT1);
	WRITE_REG(OCR1A, SERVO_MIN_IMPULSE);
	Servo_AdvanceState = ServoFirst;
}

void ServoFirst()
{
	SET_BIT(PORTB, SERVO_PIN);
	// PCK/8
	uint8_t tmp = TCCR1;
	CLEAR_BIT(tmp, CS13);
	SET_BIT(tmp, CS12);
	CLEAR_BIT(tmp, CS11);
	CLEAR_BIT(tmp, CS10);
	TCCR1 = tmp;
	
	CLEAR_REG(TCNT1);
	WRITE_REG(OCR1A, s_servo);
	Servo_AdvanceState = ServoSecond;
}

void ServoSecond()
{
	CLEAR_BIT(PORTB, SERVO_PIN);
	// PCK/256
	uint8_t tmp = TCCR1;
	SET_BIT(tmp, CS13);
	CLEAR_BIT(tmp, CS12);
	CLEAR_BIT(tmp, CS11);
	SET_BIT(tmp, CS10);
	TCCR1 = tmp;
	
	CLEAR_REG(TCNT1);
	WRITE_REG(OCR1A, SERVO_PAUSE);
	Servo_AdvanceState = ServoPause;
	if(s_spiWatchdogPrev == s_spiWatchdog)
	{
		// Reset USI counter if it's taking a while
		unsigned char bt = USISR;
		USISR = bt & ~((1 << USICNT0) | (1 << USICNT1) | (1 << USICNT2) | (1 << USICNT3));
		Package_ResetAllBuffers();
	}
	s_spiWatchdogPrev = s_spiWatchdog;
}


