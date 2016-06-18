#ifndef CONFIG_H_
#define CONFIG_H_

#define DEVICE_ATTINY2313

#ifdef DEVICE_ATTINY13
	#define CLK				PB2
	#define DBG				PB3
	#define INT				PB3
	#define DENDRITE_CSN	PB4
	#define DENDRITE_MOSI	PB3
	#define DENDRITE_MISO	PB4
	#define DENDRITE_IRQ	PB5
	
#elif defined DEVICE_ATTINY2313
	#define DENDRITE_SCK	PB7
	#define DENDRITE_CSN	PB3
	#define DENDRITE_MOSI	PB5
	#define DENDRITE_MISO	PB6
	#define DENDRITE_IRQ	PB4
	#define DENDRITE_CE		PB3
	
#endif

#define STREAM_BUFFER_SIZE  32

#endif //CONFIG_H_