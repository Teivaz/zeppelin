/**
 * Link script for the STM32WB55RG SoC
 * Created by Teivaz
 * 16.02.2020
 */

/**
 *
 * Memory sizes for STM32WB55xx
 *
 *        Part | Flash | SRAM | SRAM1 | SRAM2
 * -------------------------------------------
 * STM32WB55xC |  256K | 128K |   64K |   64K
 * STM32WB55xE |  512K | 256K |  192K |   64K
 * STM32WB55xG |    1M | 256K |  192K |   64K
 *
 */

MEMORY
{
	ram		(rwx):	ORIGIN = 0x20000000, LENGTH = 192K
	ram2a	(rwx):	ORIGIN = 0x20030000, LENGTH = 32K
	ram2b	(rwx):	ORIGIN = 0x20038000, LENGTH = 32K
	flash	(rx) :	ORIGIN = 0x08000000, LENGTH = 1M
}
_stack = ORIGIN(ram) + LENGTH(ram);
ENTRY(Reset_Handler)

SECTIONS
{
	.boot : {
		KEEP(*(.vector))
	} > flash

	.text : {
		*(.text)
		*(.text*)
		*(.glue_7) /* glue arm to thumb code */
		*(.glue_7t) /* glue thumb to arm code */

		/* C standard library */
		KEEP (*(.init))
		KEEP (*(.fini))
	} > flash

	.rodata : {
		. = ALIGN(4);
		*(.rodata*)
		*(.rodata)
		. = ALIGN(4);
	} > flash

	/* C standard library */
	.preinit_array : {
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	} > flash
	.init_array : {
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
	} > flash
	.fini_array : {
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array*))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} > flash

	_data_start_initialised = LOADADDR(.data);

	.data : {
		. = ALIGN(4);
		_data_start = .;
		*(.data)
		*(.data.*)
		. = ALIGN(4);
		_data_end = .;
	} > ram AT> flash

	.bss : {
		. = ALIGN(4);
		__bss_start = .;
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
	} > ram

	/DISCARD/ : {
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}

	.ARM.attributes 0 : {
		*(.ARM.attributes)
	}
}
