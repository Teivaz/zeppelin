#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CLK		PB2
#define DBG		PB3
#define INT		PB3
#define CSN		PB4
#define MOSI	PB1

#define STREAM_BUFFER_SIZE  32

#define PORTA_ 0x1B
#define PORTB_ 0x18
#define PORTC_ 0x15
#define PORTD_ 0x12

#define STICK_BTN_PORTS   {PORTB_,	PORTB_,	PORTB_,	PORTB_}
#define STICK_BTN_PINS    {PB0,		PB1,	PB2,	PB3}

#endif //_CONFIG_H_