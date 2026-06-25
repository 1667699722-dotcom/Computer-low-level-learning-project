.include "src/memory.inc"  
.data  ;数据区
.align 4  ;对齐到4字节边界

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
.extern store_instr_addrs
.extern get_instr_addrs
_main:
    bl memory_init 
    mov x19,#0
loop:
    mov x20,x19
    bl store_string

    mov x20,x19
    bl get_string_ptr_len_spaces 
    ;x0第一个空格位置 x1返回第二个空格位置 x2返回地址 x3返回长度

    bl store_instr_addrs

    mov x0,x19
    bl get_instr_addrs ;返回x0序列指令所储存的地址
    bl get_instr_0 ;输入是上一项的x0

    bl print_newline

    mov x0,x19
    bl get_instr_addrs 
    bl get_instr_1

    bl print_newline

    mov x0,x19
    bl get_instr_addrs 
    bl get_instr_2

    bl print_newline

    mov x0,x19
    bl get_instr_addrs 
    bl get_instr_3

    bl print_newline
    ;bl test_write_memory
    ;bl test_write_read
    ;bl test_write
    
flag:
    add x19, x19, #1
    cmp x19, #2
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
    
get_instr_0:
    stp x29,x30,[sp,#-32]!
    stp x19,x20,[sp,#16]
    mov x20,x0
    ldr x1,[x20] ;取原字符串地址
    mov x2,#0         ; 长度计数器
strlen_loop:
    ldrb w3,[x1,x2]   ; 读取字节
    cbz w3,strlen_done ; 如果是0，结束
    add x2,x2,#1      ; 否则长度+1
    b strlen_loop
strlen_done:
    mov x0,#1
    bl sys_write
    ldp x19,x20,[sp,#16]
    ldp x29,x30,[sp],#32
    ret

get_instr_1:
    stp x29,x30,[sp,#-32]!
    stp x19,x20,[sp,#16]
    mov x20,x0
    ldr x1,[x20,#8] ;取第一个参数地址
    mov x2,#0         ; 长度计数器
strlen_loop_1:
    ldrb w3,[x1,x2]   ; 读取字节
    cbz w3,strlen_done_1 ; 如果是0，结束
    add x2,x2,#1      ; 否则长度+1
    b strlen_loop_1
strlen_done_1:
    mov x0,#1
    bl sys_write
    ldp x19,x20,[sp,#16]
    ldp x29,x30,[sp],#32
    ret

get_instr_2:
    stp x29,x30,[sp,#-32]!
    stp x19,x20,[sp,#16]
    mov x20,x0
    ldr x1,[x20,#16] ;取第一个参数地址
    mov x2,#0         ; 长度计数器
strlen_loop_2:
    ldrb w3,[x1,x2]   ; 读取字节
    cbz w3,strlen_done_2 ; 如果是0，结束
    add x2,x2,#1      ; 否则长度+1
    b strlen_loop_2
strlen_done_2:
    mov x0,#1
    bl sys_write
    ldp x19,x20,[sp,#16]
    ldp x29,x30,[sp],#32
    ret

get_instr_3:
    stp x29,x30,[sp,#-32]!
    stp x19,x20,[sp,#16]
    mov x20,x0
    ldr x1,[x20,#24] ;取第一个参数地址
    mov x2,#0         ; 长度计数器
strlen_loop_3:
    ldrb w3,[x1,x2]   ; 读取字节
    cbz w3,strlen_done_3 ; 如果是0，结束
    add x2,x2,#1      ; 否则长度+1
    b strlen_loop_3
strlen_done_3:
    mov x0,#1
    bl sys_write
    ldp x19,x20,[sp,#16]
    ldp x29,x30,[sp],#32
    ret

print_newline:
    stp x29,x30,[sp,#-32]!
    strb wzr,[sp,#16]      ; 清一下
    mov w8,#10
    strb w8,[sp,#16]       ; 存 '\n' (ASCII 10)
    add x1,sp,#16          ; x1 = 缓冲区地址
    mov x0,#1              ; x0 = 标准输出
    mov x2,#1              ; x2 = 长度 1
    bl sys_write
    ldp x29,x30,[sp],#32
    ret