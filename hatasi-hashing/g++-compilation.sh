#/bin/sh

mkdir -p bin
g++ -I$HOME/valgrind-3.10.1/taintgrind/ -I$HOME/valgrind-3.10.1/include/ -Wall sources/*.cpp -o bin/hatasi-hashing
