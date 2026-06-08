.include "src/memory.inc"  
.extern heap           
.extern heap_end    
.global memory_init
.global memory_alloc

memory_init:
    adrp x0, heap@PAGE       ;拿到 heap 所在页的基地址
    add x0, x0, heap@PAGEOFF ;加上页内偏移，得到最终真实地址
    mov x1, #HEAP_SIZE ;x1 = 堆总大小
    sub x1, x1, #8    ;x1 = 可用空间（总大小 - 8）
    str w1, [x0]     ;将寄存器 w1 的值存储到 x0 指向的内存地址
    mov w2, #0       ;将立即数 0 移动到寄存器 w2 中
    str w2, [x0,#4]  ;将寄存器 w2 的值存储到 x0 + 4 的内存地址
    ret
memory_alloc:
    stp x19,x20,[sp,#-32]!;预索引寻址把 x19 和 x20 存储到这个新地址
    stp x21,x22,[sp,#16]  ;偏移寻址把 x21 和 x22 存储到这个地址
    mov x19,x0    ;存入请求分配的大小
    adrp x20, heap@PAGE          
    add x20, x20, heap@PAGEOFF ;存入heap指针
    adrp x21, heap_end@PAGE      
    add x21, x21, heap_end@PAGEOFF ;迅如heapend指针
alloc_loop:
    cmp x20,x21    ;比较heap_i与heapend的大小
    b.hs alloc_fail    ;如果heapi超过heapend就分配0字节

    ldr w22, [x20]    ;从heapi位置读取四个字节  
    ldr w23, [x20, #4]  ;从heapi+4位置读取四个字节  

    cmp w23, #1        ;看此heap是否为空    
    b.eq next_block    ;非空跳到下一个区块
    cmp w22, w19        ;比较heapi区块大小与预分配大小     
    b.lt next_block     ;heapi区块小于预分配大小则跳到下一个区块
    
    mov w0,#1  ;在x0的地位放入1
    str w0,[x20,#4] ;将寄存器w0的值存储到x20+4的位置
    sub x1,x22,x19   ;在x1存入heapi大小与预分配大小的差值
    cmp x1,#8  ;比较剩余区块大小是否小于8字节
    b.le no_split  ;如果剩余区块小于8字节直接把剩下的打包分配给申请
    add x2,x20,#8  ;
    add x2,x2,x19  ;在x2存入到分配完区块的下一个heap位置
    sub x3,x1,#8  ;剩余空余的大小
    str w3,[x2]  ;在下一个heap位置存入剩余空间的大小
    mov w4,#0   ;
    str w4,[x2,#4]  ;在下一个heap+4位置放入空0字节

    str w19,[x20] ;在heap位置存入预分配的大小
no_split:
    add x0,x20,#8  ;在x0存入预分配区块数据头指针
    b alloc_done  ;进入完成
next_block:
    add x20, x20, #8   ;        
    add x20, x20, x22    ; 移动到下一个heap头位置   
    b alloc_loop   ;
alloc_fail:
    mov x0,#0   ;
alloc_done:
    ldp x21, x22, [sp, #16]    ;退栈
    ldp x19, x20, [sp], #32   ;退栈
alloc_done_flag:
    ret