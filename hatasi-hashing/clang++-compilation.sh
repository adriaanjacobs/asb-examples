#/bin/sh

CLANG_HOME=$HOME/clang-llvm/build

mkdir -p bin
$CLANG_HOME/bin/clang++ -Xclang -load -Xclang $CLANG_HOME/lib/DetectPtrCasts.so -Xclang -add-plugin -Xclang detect-ptr-casts -Xclang -plugin-arg-detect-ptr-casts -Xclang p2i sources/*.cpp
