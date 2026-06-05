clang -g -o bin/hello hello.s src/*.s -nostdlib -lSystem
lldb ./bin/hello
