.include "src/memory.inc"  
.extern heap           
.extern heap_end 
.extern input_buf
.extern output_buf
.extern memory_alloc
.global copy_string

copy_string:
   stp x29,x30,[sp,#-32]!
   stp x19,x20,[sp,#16]

   mov x19,x0 ;记录输入字节数目
   sub x19,x19,#1
   adrp x20, input_buf@PAGE
   add x20, x20, input_buf@PAGEOFF   
   bl memory_alloc
   mov x2,#0
copy_loop:
   cmp x2,x19
   b.ge copy_done
   ldrb w4,[x20,x2]
   strb w4,[x0,x2]
   add x2,x2,#1
   b copy_loop
copy_done:
   add x19,x19,#1
   add x2,x2,#1
   strb wzr,[x0,x19]
   ldp x19, x20, [sp, #16]
   ldp x29, x30, [sp], #32
   ret

    

