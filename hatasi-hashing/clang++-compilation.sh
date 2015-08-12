#/bin/sh

mkdir -p bin
clang++ -fsanitize=memory -Wall sources/*.cpp -o bin/hatasi-hashing
