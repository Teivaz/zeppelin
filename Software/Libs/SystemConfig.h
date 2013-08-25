#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H

#include <avr/eeprom.h>

// Letters to identify device is stored in EEPROM memory
// To access them in runtime on initialization phase
// InitLetters should be called
char e_primaryLetter EEMEM = 'Z';
char e_secondaryLetter EEMEM = '1';

#define RADIO_FREQUENCY	0

char PRIMARY_LETTER;
char SECONDARY_LETTER;

#define InitLetters() \
	PRIMARY_LETTER = eeprom_read_byte(&e_primaryLetter);\
	SECONDARY_LETTER = eeprom_read_byte(&e_secondaryLetter);

#endif /* _SYSTEM_CONFIG_H */