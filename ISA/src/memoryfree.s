.include "src/memory.inc"  
.extern heap           
.extern heap_end 
.global memory_free

memory_free:
    cbz x0,free_done
    sub x0,x0,#8
    mov w1,#0
    str w1,[x0,#4]
free_done:
    ret
