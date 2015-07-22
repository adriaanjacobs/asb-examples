import os
os.system("clang -fsanitize=memory -Wall  sources/uninitialized-struct.c -o bin/uninitialized-struct") 
