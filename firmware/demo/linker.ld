/* Custom linker script for LiteX on Tang Nano 20K */
MEMORY {
    rom         (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00008000 /* Your firmware will be placed here by linker */
    sram        (rwx) : ORIGIN = 0x01000000, LENGTH = 0x00002000
    main_ram (rwx) : ORIGIN = 0x40000000, LENGTH = 0x00800000 /* This is where the BIOS will copy your program for execution */
}

ENTRY(_start) /* LiteX BIOS provides _start, which eventually calls main() */

SECTIONS {
    /* Code and read-only data go into the ROM. */
    /* objcopy will extract this starting from address 0 for the .bin file. */
    .text : {
        *(.text .text.*)
        *(.rodata .rodata.*)
    } > rom

    /* Initialized data goes into the ROM. */
    /* The BIOS will copy this from ROM to main_ram during startup. */
    .data : {
        *(.data .data.*)
    } > rom

    /* Uninitialized data (BSS) goes directly into main_ram (RAM) and is cleared by the BIOS. */
	.bss (NOLOAD) : {
        . = ALIGN(16);
        _bss_start = .; /* Add this line */
        *(.bss .bss.*)
        *(COMMON)
        _bss_end = .;   /* Add this line */
    } > main_ram

    /* Stack goes into main_ram (RAM). */
    .stack (NOLOAD) : {
        . = ALIGN(16);
        . += 0x1000; /* Allocate 4KB for stack */
        _stack = .;
    } > main_ram

    /* Heap starts after BSS and Stack in main_ram. */
    _heap_start = .;
    _heap_end = ORIGIN(main_ram) + LENGTH(main_ram);
}