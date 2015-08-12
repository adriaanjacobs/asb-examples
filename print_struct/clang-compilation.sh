#/bin/sh
mkdir -p bin
clang -fsanitize=memory -Wall  sources/print_struct.c -o bin/print_struct
