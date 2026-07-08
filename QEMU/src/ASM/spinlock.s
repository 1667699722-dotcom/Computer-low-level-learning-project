.global spinlock_init
.global spinlock_acquire
.global spinlock_release

spinlock_init:
    str wzr, [x0]
    ret

spinlock_acquire:
1:
    ldxr w1,[x0]
    cbnz w1,1b
    mov w1,#1
    stxr w2,w1,[x0]
    cbnz w2,1b
    dmb ish
    ret

spinlock_release:
    dmb ish
    str wzr, [x0]
    ret
