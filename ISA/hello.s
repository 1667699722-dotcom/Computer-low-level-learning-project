.include "src/memory.inc"  
.data  ;数据区
.align 4  ;

.global heap
.global heap_end
.extern hello_msg
.extern hello_len

heap:                  ;堆内存的开始位置
    .space HEAP_SIZE   ;开辟一块长度为HEAP_SIZE的内存空间
heap_end:              ;堆内存的结束位置
.global input_buf
input_buf:.space 256
.global output_buf
output_buf:.space 256

.text 
.global _main
.extern exit
.extern memory_init
.extern memory_alloc
.extern memory_free
.extern sys_write
.extern sys_read
.extern test_write
.extern test_memory 
.extern test_read
.extern test_write_read
.extern test_write_memory
_main:
    bl memory_init 
    mov x19,#0

loop:
    bl test_read
    bl copy_string
    bl test_write_memory
    bl test_write_read
    ;bl test_write
  
    add x19, x19, #1
    cmp x19, #1
    b.eq do_exit
    b loop  
 
do_exit:
    b exit

