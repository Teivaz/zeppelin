#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "types.h"
#include "SystemConfig.h"

volatile unsigned char position = 40;	//7...40

volatile uint8_t s_servo = 0;

volatile uint8_t s_motorA = 0;
volatile uint8_t s_motorB = 0;

char s_spiState = 0;

#define MOTOR_PIN_A PB3
#define MOTOR_PIN_B	PB4

int main(void)
{
	Init();
    while(1)
    {
		;
    }
}

void Init()
{
	InitLetters;
	// Default frequency is 8.0 MHz divided by 1
	// System frequency = 8.0 MHz
	
	/* === Configure prescale here ===	*/
	uint8_t clkpr = (0 & CLKPS0)|
					(0 & CLKPS1)|
					(0 & CLKPS2)|
					(0 & CLKPS3);
	WRITE_REG(CLKPR, CLKPCE);
	WRITE_REG(CLKPR, clkpr);
		
	/* === Configure timer here === */	
	// Stop timers
	SET_BIT(GTCCR, TSM);
	{
		SET_BIT(TCCR0B, CS00); // Set source Fcpu/64
		SET_BIT(TCCR0B, CS01);
		
		SET_BIT(TIMSK, OCIE0A); // Interrupt on compare match A
		SET_BIT(TIMSK, OCIE0B); // Interrupt on compare match B
		SET_BIT(TIMSK, TOV0); // Interrupt on timer overflow
			
		WRITE_REG(OCR0A, s_motorA);
		WRITE_REG(OCR0B, s_motorB);
	}
	// start timers
	CLEAR_BIT(GTCCR, TSM);	
	
	// The rising edge of INT0 generates an interrupt request.
	SET_BIT(MCUCR, ISC00);
	SET_BIT(MCUCR, ISC01);
	SET_BIT(GIMSK, INT0); // Enable INT0 interrupt
	
	SET_BIT(USICR, USIWM0);
	SET_BIT(USICR, USICS1);
	
	SET_BIT(PORTB, MOTOR_PIN_A);
	SET_BIT(PORTB, MOTOR_PIN_B);
	SET_BIT(DDRB, MOTOR_PIN_A);
	SET_BIT(DDRB, MOTOR_PIN_B);
	
	sei();
}

// === SPI ===
ISR(INT0_vect)
{
	ReadSpi();
}

void ReadSpi()
{
	if(s_spiState == 0)
	{
		if(USIDR == PRIMARY_LETTER)
		{
			++s_spiState;
		}
	}
	else if(s_spiState < 9)
	{
		++s_spiState;
	}
	else if(s_spiState == 9)
	{
		if(USIDR == SECONDARY_LETTER)
		{
			++s_spiState;
		}
		else
		{
			s_spiState = 0;
		}
	}
	else if(s_spiState < 18)
	{
		++s_spiState;
	}
	else if(s_spiState == 18)
	{
		SetMotorSpeedSigned(USIDR);
		++s_spiState;
	}
	else if(s_spiState < 27)
	{
		++s_spiState;
	}
	else if(s_spiState == 27)
	{		
		SetServoPosition(USIDR);
		s_spiState = 0;
	}
}

// === Servo controls ===
void SetServoPosition(unsigned char position)
{
	
}

// === DC motor controls ===
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
