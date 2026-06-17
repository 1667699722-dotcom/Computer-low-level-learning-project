clang -S -falign-functions=4 -mstrict-align src/main.c -o ctos/main.s
clang -g -o bin/main ctos/main.s -nostdlib -lSystem
lldb ./bin/main
