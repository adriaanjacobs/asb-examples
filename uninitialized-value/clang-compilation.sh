#/bin/sh
mkdir -p bin
clang -fsanitize=memory -Wall  sources/uninitialized-struct.c -o bin/uninitialized-struct
