/*
Link script for the STM32L011F4 MCU
Created by Teivaz
14.01.2020
*/

MEMORY
{
	ram (xrw) : ORIGIN = 0x20000000, LENGTH = 2K
	eeprom (rw) : ORIGIN = 0x08080000, LENGTH = 512
	flash (rx) : ORIGIN = 0x08000000, LENGTH = 16K
}
PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));

EXTERN(vector_table);
ENTRY(reset_handler);

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
		*(.data)
		*(.data.*)
		. = ALIGN(4);
	} > ram AT> flash

	.bss : {
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss)
		*(.bss.*)
    *(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
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
