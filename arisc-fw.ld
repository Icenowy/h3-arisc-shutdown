OUTPUT_FORMAT("elf32-or1k")
OUTPUT_ARCH(or1k)
ENTRY(_start)
SECTIONS
{
	. = 0x00000000;

	.text : {
		start.o (.text.start)
		*(.text*)
		. = ALIGN(4);
	}

	.rodata : { 
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) 
		. = ALIGN(4);
	}

	.data : {
		*(.data*)
		. = ALIGN(4);
	}

	.bss : {
		__bss_start = .;       
		*(.bss*)
		. = ALIGN(4);
		__bss_end = .;
	}

	/DISCARD/ : { *(.comment*) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
	/DISCARD/ : { *(.note*) }
	/DISCARD/ : { *(.eh_frame*) }
}
