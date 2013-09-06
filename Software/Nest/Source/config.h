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

//						    F-L-S	F-L-M	B-R-S	B-R-M
#define STICK_BTN_PORTS    {PORTB_,	PORTB_,	PORTB_,	PORTB_}
#define STICK_BTN_PINS     {PB0,	PB1,	PB2,	PB3}
#define STICK_ADC_PORTS	   {0, 1,	2, 3,	4, 5,	6, 7}

#define MODE_BTN_PORT	PORTB
#define MODE_BTN_PIN	PB1

#define CAL_BTN_PORT	PORTB
#define CAL_BTN_PIN		PB1

#endif //_CONFIG_H_