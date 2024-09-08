/*
Link script for the STM32L011F4 MCU
Created by Teivaz
14.01.2020
*/

MEMORY
{
	ram (xrw) : ORIGIN = 0x20000000, LENGTH = 2K
	flash (rx) : ORIGIN = 0x08000000, LENGTH = 16K
	eeprom (rw) : ORIGIN = 0x08080000, LENGTH = 512
}
PROVIDE(__stack = ORIGIN(ram) + LENGTH(ram));

SECTIONS
{
	.text : {
		KEEP(*(.vectors)) /* Skip optimization for the interrup vectors */
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata.*)
		. = ALIGN(4);
	} > flash

	.data : {
		. = ALIGN(4);
		*(.data)
		*(.data.*)
		. = ALIGN(4);
	} > ram AT> flash

	. = ALIGN(4);
	.bss : {
		__bss_start = .;
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
	} > ram

	.eeprom : {
		. = ALIGN(4);
		*(.eeprom)
		. = ALIGN(4);
	} > eeprom

	.ARM.attributes 0 : {
		*(.ARM.attributes)
	}
}
