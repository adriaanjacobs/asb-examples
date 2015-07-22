import os
os.system("clang -fsanitize=memory -Wall  sources/padded-struct.c -o bin/padded-struct") 
