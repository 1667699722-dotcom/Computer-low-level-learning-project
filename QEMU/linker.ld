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
    }

    .exception_vector_table 0x40001000 : ALIGN(4K) {
        *(.text.exceptions)   
    }

    .text : ALIGN(4K) {
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
    . += 8388608;  
    heap_end = .;

    __heap_start = .;
    __heap_end = ORIGIN(RAM) + LENGTH(RAM) - 128K;
    
    . = ALIGN(16);
    __stack_top = .;
    . += 32768;   
    __stack_top_cpu1 = .;
    . += 32768;   
    __stack_top_cpu2 = .;
    . += 32768; 
    __stack_top_cpu3 = .;
    . += 32768; 

    __vheap_start = 0x50000000;
    __vheap_end = 0x58000000;
}
