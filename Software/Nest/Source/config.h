#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CLK		PB2
#define DBG		PB3
#define INT		PB3
#define CSN		PB4
#define MOSI	PB1

#define STREAM_BUFFER_SIZE  33

#define QUERY_PERIOD 20	// time in ms

#define PORTA_ 0x1B
#define PORTB_ 0x18
#define PORTC_ 0x15
#define PORTD_ 0x12

#define STICK_BTN_PORTS   {PORTB_,	PORTB_,	PORTB_,	PORTB_}
#define STICK_BTN_PINS    {PB0,		PB1,	PB2,	PB3}

#define STICK_ADC_PORTS	{0, 1,  /* EFrontLeftServo x, y*/\
						 2, 3,  /* EFrontLeftMotor x, y*/\
						 4, 5,  /* EBackRightServo x, y*/\
						 6, 7}	/* EBackRightMotor x, y*/

#endif //_CONFIG_H_