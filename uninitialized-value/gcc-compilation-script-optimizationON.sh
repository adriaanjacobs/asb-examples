#/bin/sh
mkdir -p bin
gcc -Wall -O1 sources/uninitialized-struct.c -o bin/uninitialized-struct
