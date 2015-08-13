#/bin/sh
mkdir -p bin
gcc -Wall -O0 sources/uninitialized-struct.c -o bin/uninitialized-struct
