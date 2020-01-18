/*
Link script for the STM32L011F4 MCU
Created by Teivaz
14.01.2020
*/

MEMORY
{
	ram (xrw) : ORIGIN = 0x20000000, LENGTH = 2K
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
	} > ram

	.bss : {
		*(.bss)
		*(.bss.*)
		. = ALIGN(4);
	} > ram

	.ARM.attributes 0 : {
		*(.ARM.attributes)
	}
}
