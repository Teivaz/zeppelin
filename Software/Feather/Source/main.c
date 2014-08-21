#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "types.h"
#include "utils.h"
#include "SystemConfig.h"
#include "package.h"

#define SERVO_MIN_IMPULSE	100	// This value should be around 0.5 ms. 1 equals to 0.005 ms
#define SERVO_PAUSE			80	// This value should be around 20 ms. 1 equals 0.25 ms 

volatile uint8_t s_servo = 63;	// 1 equals 0.0078 ms (78 us) + 0.5 ms
volatile uint8_t s_motorA = 0;
volatile uint8_t s_motorB = 0;

unsigned char s_spiWatchdog = 1;
unsigned char s_spiWatchdogPrev = 0;

void S_ServoPause();
void S_ServoFirst();
void S_ServoSecond();

volatile char s_servoState = EServoPause;
void(*s_servoStatePtr)() = S_ServoPause;

void _dbg()
{
	#if 0 // Spokes
	char a = READ_BIT(PORTB, SERVO_PIN) != 0;
	if(a)
	{
		CLEAR_BIT(PORTB, SERVO_PIN);
		SET_BIT(PORTB, SERVO_PIN);
	}
	else
	{
		SET_BIT(PORTB, SERVO_PIN);
		CLEAR_BIT(PORTB, SERVO_PIN);
	}
	#endif
}

int main(void)
{
	Init();
    while(1)
    {
		if(Package_IsDirty() > 0)
		{
			Package_Process();
		}
		if(Package_PayloadDetected() == 1)
		{
			SetMotorSpeedSigned(Package_GetData(0));
			SetServoPosition(Package_GetData(1));
		}
		
    }
}

void Init()
{
	s_servoStatePtr = S_ServoPause;
	Package_Init();
	InitLetters();
	// Default frequency is 8.0 MHz divided by 1
	// System frequency = 8.0 MHz
	
	/* === Configure prescale here ===	*/
	/*
	uint8_t clkpr = (0 << CLKPS0)|
					(0 << CLKPS1)|
					(0 << CLKPS2)|
					(0 << CLKPS3);
	WRITE_REG(CLKPR, 1 << CLKPCE);
	WRITE_REG(CLKPR, clkpr);
	*/
		
	/* === Configure timer 0 here === */	
	// Stop timers
	SET_BIT(GTCCR, TSM);
	{
		SET_BIT(TCCR0B, CS00); // Set source Fcpu/8
		SET_BIT(TCCR0B, CS01);
		
		SET_BIT(TIMSK, OCIE0A); // Interrupt on compare match A
		SET_BIT(TIMSK, OCIE0B); // Interrupt on compare match B
		SET_BIT(TIMSK, TOV0); // Interrupt on timer overflow
			
		WRITE_REG(OCR0A, s_motorA);
		WRITE_REG(OCR0B, s_motorB);
		

		SET_BIT(TCCR1, CS13);
		SET_BIT(TCCR1, CS10);
		WRITE_REG(OCR1A, SERVO_PAUSE);
		
		SET_BIT(TIMSK, OCIE1A);
		SET_BIT(PLLCSR, LSM);
		SET_BIT(PLLCSR, PCKE);
		SET_BIT(PLLCSR, PLLE);
	}
	// start timers
	CLEAR_BIT(GTCCR, TSM);	
	
#if 0
	// The rising edge of INT0 generates an interrupt request.
	SET_BIT(MCUCR, ISC00);
	SET_BIT(MCUCR, ISC01);
	SET_BIT(GIMSK, INT0); // Enable INT0 interrupt
#endif
	
	SET_BIT(USICR, USIWM1);	// Two wire mode
	SET_BIT(USICR, USICS1); // Clock - external positive edge 
	SET_BIT(USICR, USIOIE); // Interrupt on counter overflow
	SET_BIT(USISR, USICNT0); //
	
	
	SET_BIT(PORTB, MOTOR_PIN_A);
	SET_BIT(PORTB, MOTOR_PIN_B);
	SET_BIT(PORTB, SERVO_PIN);
	
	SET_BIT(DDRB, MOTOR_PIN_A);
	SET_BIT(DDRB, MOTOR_PIN_B);
	SET_BIT(DDRB, SERVO_PIN);
	
	sei();
}

#pragma mark "SPI"
ISR(INT0_vect) // When we have activity on clock input
{
	//++s_spiWatchdog;
}

ISR(USI_OVF_vect) // When SPI buffer is full
{
	++s_spiWatchdog;
	PackageI_OnReceived(USIBR);
	SET_BIT(USISR, USIOIF); // Set 1 to clear interrupt
	SET_BIT(USISR, USICNT0); // 
}

#pragma mark "Servo"
void SetServoPosition(unsigned char position)
{
	char tmp = max(2, position);
	tmp = min(200, tmp);
	cli();
	// clamp 2...200
	s_servo = tmp;
	sei();
}

void AdvanceServoState()
{
	s_servoStatePtr();
}

void S_ServoPause()
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
	s_servoStatePtr = S_ServoFirst;
}

void S_ServoFirst()
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
	s_servoStatePtr = S_ServoSecond;
}

void S_ServoSecond()
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
	s_servoStatePtr = S_ServoPause;
	if(s_spiWatchdogPrev == s_spiWatchdog)
	{
		// Reset USI counter if it's taking a while
		unsigned char bt = USISR;
		USISR = bt & ~((1 << USICNT0) | (1 << USICNT1) | (1 << USICNT2) | (1 << USICNT3));
		Package_ResetAllBuffers();
	}
	s_spiWatchdogPrev = s_spiWatchdog;
}

#pragma mark "DC Motor"
// -128 - max CCW, 0 - stop, 127 - max CW
void SetMotorSpeedSigned(signed char speed)
{
	cli();
	if (speed == 0)
	{
		s_motorA = 0;
		s_motorB = 0;
	}
	else if(speed > 0)
	{
		uint8_t newSpeed = speed;
		s_motorA = newSpeed << 1;
		s_motorB = 0;
	}
	else
	{
		uint8_t newSpeed = -speed;
		s_motorA = 0;
		s_motorB = newSpeed << 1;
	}
	sei();
}

// 0 - max CCW, 127 - stop, 255 - max CW
void SetMotorSpeedUnsigned(unsigned char speed)
{
	cli();
	if(speed > 127)
	{
		uint8_t newSpeed = speed - 127;
		s_motorA = newSpeed << 1;
		s_motorB = 0;		
	}
	else
	{
		uint8_t newSpeed = 127 - speed;
		s_motorA = 0;
		s_motorB = newSpeed << 1;
	}
	sei();
}

ISR(TIM0_COMPA_vect)
{
	CLEAR_BIT(PORTB, MOTOR_PIN_A);
}
ISR(TIM0_COMPB_vect)
{
	CLEAR_BIT(PORTB, MOTOR_PIN_B);
}

ISR(TIM0_OVF_vect)
{
	if(s_motorA != 0)
		SET_BIT(PORTB, MOTOR_PIN_A);
	if(s_motorB != 0)
		SET_BIT(PORTB, MOTOR_PIN_B);
	
	WRITE_REG(OCR0A, s_motorA);
	WRITE_REG(OCR0B, s_motorB);
}

ISR(TIM1_COMPA_vect)
{
	
#if 0 // Spokes for debug timing
	char a = READ_BIT(PORTB, SERVO_PIN) != 0;
	if(a)
	{
		CLEAR_BIT(PORTB, SERVO_PIN);
		SET_BIT(PORTB, SERVO_PIN);
	}
	else
	{
		SET_BIT(PORTB, SERVO_PIN);
		CLEAR_BIT(PORTB, SERVO_PIN);
	}
#endif
	AdvanceServoState();
}
