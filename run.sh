

clang -o PCB_TCB/build/kernel   PCB_TCB/kernel.c PCB_TCB/src/*.c -IPCB_TCB/include -Wno-deprecated-declarations

./PCB_TCB/build/kernel