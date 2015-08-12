#/bin/sh

mkdir -p bin
g++ -I$HOME/valgrind-3.10.1/taintgrind/ -I$HOME/valgrind-3.10.1/include/ -g -O0 -Wall sources/*.cpp -o bin/hatasi-hashing
