#include <stdio.h>
#include "hatasi.h"
#include <stdlib.h>
int main(int argc, char** argv) {
        printf("sizeof(int*) = %ld\n", sizeof(int*));
        printf("sizeof(unsigned int) = %ld\n", sizeof(unsigned int));
        printf("sizeof(unsigned long) = %ld\n", sizeof(unsigned long));
        printf("sizeof(size_t) = %ld\n", sizeof(size_t));
        printf("\n");
        
        Hatasi ht;
        
        for (int i = 0; i < argc; ++i) {
                ht.put(argv[i], i);
        }
        
        for (int i = 0; i < argc; ++i) {
                printf("%s  -->  %d\n", argv[i], ht.get(argv[i]));
        }

        return 0;
}
