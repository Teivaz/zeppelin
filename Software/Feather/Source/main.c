#include "main.h"
#include <avr/interrupt.h>
#include "types.h"
#include "utils.h"
#include "SystemConfig.h"
#include "package.h"
#include "servo.h"
#include "motor.h"
#include "config.h"

#ifndef STORE_LETTERS_IN_FLASH
	uint8_t e_primaryLetter EEMEM = 'Z';
	uint8_t e_secondaryLetter EEMEM = '1';
#endif //STORE_LETTERS_IN_FLASH

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
		Package_Process();
		char* payload = Package_GetPayload();
		if(payload)
		{
			Motor_SetSpeedSigned(payload[0]);
			Servo_SetPosition(payload[1]);
			Package_ClearState();
		}
    }
}

void Init()
{
	Servo_Init();
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

		Motor_InitTimer();

		SET_BIT(TCCR1, CS13);
		SET_BIT(TCCR1, CS10);
		Servo_InitTimer();
		
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

// SPI
ISR(INT0_vect) // When we have activity on clock input
{
}

ISR(USI_OVF_vect) // When SPI buffer is full
{
	PackageI_OnReceived(USIDR);
	SET_BIT(USISR, USIOIF); // Set 1 to clear interrupt
	SET_BIT(USISR, USICNT0); // 
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
	Motor_Tick();
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
	Servo_AdvanceState();
}
