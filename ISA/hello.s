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
.extern store_string_ptr_len
.extern get_string_ptr
.extern get_string_len
.extern store_ptr_done
.extern print_index_strings
.extern find_two_spaces
_main:
    bl memory_init 
    mov x19,#0
loop:
    mov x20,#0
    bl store_string

    mov x20,#0
    bl get_string_ptr_len_spaces 
    ;x0第一个空格位置 x1返回第二个空格位置 x2返回地址 x3返回长度
    ;bl test_write_memory
    ;bl test_write_read
    ;bl test_write
  
    add x19, x19, #1
    cmp x19, #1
    b.eq do_exit
    b loop  
 
do_exit:
    b exit

store_string:
    stp x29, x30, [sp, #-16]!
    bl test_read
    bl copy_string
    mov x1,x20
    bl store_string_ptr_len
    ldp x29, x30, [sp], #16
    ret
print_string:
    stp x29, x30, [sp, #-16]!
    mov x0,x20
    bl print_index_strings
print_string_done_flag:
    ldp x29, x30, [sp], #16
    ret
get_string_ptr_len_spaces:
    stp x29, x30, [sp, #-16]!
    bl print_string ;x1返回地址 x2返回长度
    stp x1, x2, [sp, #-16]! 
    bl find_two_spaces ;x0第一个空格位置 x1返回第二个空格位置 x2返回地址 x3返回长度
    ldp x2, x3, [sp], #16  
get_string_ptr_len_spaces_done:
    ldp x29, x30, [sp], #16
    ret