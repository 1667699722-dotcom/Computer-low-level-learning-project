.data
.align 4

.equ MAX_STRINGS,32

string_count:.word 0
string_ptrs:.space MAX_STRINGS*8
string_lens:.space MAX_STRINGS*4

.text 
.global store_string_ptr_len
.global store_ptr_done
.global get_string_ptr
.global get_string_len
.global print_index_strings
.extern sys_write

store_string_ptr_len:
    stp x29,x30,[sp,#-16]!
    mov x29,sp
    cmp x1,#MAX_STRINGS
    b.hs store_ptr_done
    adrp x4, string_ptrs@PAGE
    add x4, x4, string_ptrs@PAGEOFF
    adrp x3, string_lens@PAGE
    add x3, x3, string_lens@PAGEOFF
    str x0, [x4, x1, lsl #3]
    lsl x5,x1,#2
    str w2, [x3, x5]
store_ptr_done:
    ldp x29, x30, [sp], #16
    ret
store_string_len:

get_string_ptr:

get_string_len:


print_index_strings:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    cmp x0, #MAX_STRINGS
    b.hs print_index_strings_done

    adrp x3, string_ptrs@PAGE
    add x3, x3, string_ptrs@PAGEOFF
    ldr x1, [x3, x0, lsl #3]

    adrp x3, string_lens@PAGE
    add x3, x3, string_lens@PAGEOFF
    lsl x4, x0, #2
    ldr w2, [x3, x4]

    cbz x1, print_index_strings_done
    cbz x2, print_index_strings_done

    mov x0, #1
    bl sys_write

print_index_strings_done:
    ldp x29, x30, [sp], #16
    ret




