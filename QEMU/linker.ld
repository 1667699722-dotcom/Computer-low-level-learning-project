ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x40000000, LENGTH = 64M
}

SECTIONS
{
    . = ORIGIN(RAM);

    .text : ALIGN(4K) {
        *(.text.startup)
        
        *(.text.exceptions)
        
        *(.text)
    }

    .rodata : ALIGN(4K) {
        *(.rodata)
        *(.rodata.*)
    }

    .data : ALIGN(4K) {
        *(.data)
    }

    .bss : ALIGN(4K) {
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    }

    . = ALIGN(4K);
    heap = .;
    . += 4096;
    heap_end = .;
    __heap_start = .;

    __heap_end = ORIGIN(RAM) + LENGTH(RAM) - 16K;
    . = ALIGN(16);
    __stack_top = .;
}