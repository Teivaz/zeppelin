#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CLK		PB7 // Will be driven via hardware
#define CSN		PB4	// do not change
#define MOSI	PB5 // Will be driven via hardware
#define INT		PB3 // Do we need this?
#define DBG		PB3
#define CE		PB2

#define STREAM_BUFFER_SIZE  64

#define QUERY_PERIOD 100	// time in ms

#define PORTA_ 0x1B
#define PORTB_ 0x18
#define PORTC_ 0x15
#define PORTD_ 0x12

//						    F-L-S	F-L-M	B-R-S	B-R-M
#define STICK_BTN_PORTS    {PORTB_,	PORTB_,	PORTB_,	PORTB_}
#define STICK_BTN_PINS     {PB0,	PB1,	PB2,	PB3}
#define STICK_ADC_PORTS	   {0, 1,	2, 3,	4, 5,	6, 7}
	
#define STICK_FLM_LED_PORT PORTB
#define STICK_FLM_LED_PIN  PB1
#define STICK_FLS_LED_PORT PORTB
#define STICK_FLS_LED_PIN  PB1
#define STICK_BRM_LED_PORT PORTB
#define STICK_BRM_LED_PIN  PB1
#define STICK_BRS_LED_PORT PORTB
#define STICK_BRS_LED_PIN  PB1

#define MODE_LED_PORT	PORTB
#define MODE_LED_PIN	PB1
#define MODE_BTN_PORT	PORTB
#define MODE_BTN_PIN	PB1

#define CAL_BTN_TIME	50
#define CAL_BTN_PORT	PORTB
#define CAL_BTN_PIN		PB1
#define CAL_LED_PORT	PORTB
#define CAL_LED_PIN		PB1


#endif //_CONFIG_H_