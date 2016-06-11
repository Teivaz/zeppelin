#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H

#include <avr/eeprom.h>

// Letters to identify device is stored in EEPROM memory
// To access them in runtime on initialization phase
// InitLetters should be called

#if 0 // Copy to main.cpp
#ifndef STORE_LETTERS_IN_FLASH
	uint8_t e_primaryLetter EEMEM = 'Z';
	uint8_t e_secondaryLetter EEMEM = '1';
#endif //STORE_LETTERS_IN_FLASH
#endif

#define SPI_WORD_SIZE 8

#define RADIO_FREQUENCY	0

char PRIMARY_LETTER;
char SECONDARY_LETTER;

#ifdef STORE_LETTERS_IN_FLASH
#define InitLetters() \
	PRIMARY_LETTER = 'Z';\
	SECONDARY_LETTER = '1'
#else //STORE_LETTERS_IN_FLASH
#define InitLetters() \
	PRIMARY_LETTER = eeprom_read_byte(&e_primaryLetter);\
	SECONDARY_LETTER = eeprom_read_byte(&e_secondaryLetter)
#endif //STORE_LETTERS_IN_FLASH

#endif /* _SYSTEM_CONFIG_H */