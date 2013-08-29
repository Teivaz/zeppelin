#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "types.h"
#include "utils.h"
#include "SystemConfig.h"

volatile char s_spiBuffer[4] = {0};
// s_spiBuffer[0] - primary letter
// s_spiBuffer[1] - secondary letter
// s_spiBuffer[2] - DC motor speed
// s_spiBuffer[3] - servo position
// 5th byte corresponds CRC

volatile unsigned char position = 40;	//7...40

volatile uint8_t s_servo = 0;

volatile char s_servoState = 0;
volatile char s_servo0 = 0;
volatile char s_servo1 = 0;
volatile char s_servo2 = 0;
volatile char s_servo3 = 0;

volatile uint8_t s_motorA = 0;
volatile uint8_t s_motorB = 0;

char s_spiState = 0;

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
	InitLetters();
	// Default frequency is 8.0 MHz divided by 1
	// System frequency = 8.0 MHz
	
	/* === Configure prescale here ===	*/
	uint8_t clkpr = (0 & CLKPS0)|
					(0 & CLKPS1)|
					(0 & CLKPS2)|
					(0 & CLKPS3);
	WRITE_REG(CLKPR, CLKPCE);
	WRITE_REG(CLKPR, clkpr);
		
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

#pragma mark "SPI"
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
			s_spiBuffer[0] = USIDR;
			++s_spiState;
		}
	}
	else if(s_spiState < SPI_WORD_SIZE * 1)
	{
		++s_spiState;
	}
	else if(s_spiState == SPI_WORD_SIZE * 1)
	{
		if(USIDR == SECONDARY_LETTER)
		{
			s_spiBuffer[1] = USIDR;
			++s_spiState;
		}
		else
		{
			s_spiState = 0;
		}
	}
	else if(s_spiState < SPI_WORD_SIZE * 2)
	{
		++s_spiState;
	}
	else if(s_spiState == SPI_WORD_SIZE * 2)
	{
		s_spiBuffer[2] = USIDR;
		++s_spiState;
	}
	else if(s_spiState < SPI_WORD_SIZE * 3)
	{
		++s_spiState;
	}
	else if(s_spiState == SPI_WORD_SIZE * 3)
	{
		s_spiBuffer[3] = USIDR;
		++s_spiState;
	}
	else if(s_spiState < SPI_WORD_SIZE * 4)
	{
		++s_spiState;
	}
	else if(s_spiState == SPI_WORD_SIZE * 4)
	{
		if(CRC(s_spiBuffer, 4) == USIDR)
		{
			SetMotorSpeedSigned(s_spiBuffer[2]);
			SetServoPosition(s_spiBuffer[3]);
		}
		s_spiState = 0;
	}
}

#pragma mark "Servo"
void SetServoPosition(unsigned char position)
{
	
}

void ServoNextState()
{
	switch(s_servoState)
	{
		case 0:	// Pin is low
		break;
		
		case 1: // Pin high. Longest pause
		break;
		
		case 2: // Pin high. Shorter pause
		break;
		
		case 3: // Pin high. Short pause
		break;
		
		case 4: // Pin high. Shortest pause
		break;
	}
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
