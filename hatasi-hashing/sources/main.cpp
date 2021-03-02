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

        int cnt = 9999;
        int keys[cnt];
        
        for (int i = 0; i < cnt; ++i) {
            keys[i] = i;
            ht.put(keys+i, i*2);
        }
        
        // for (int i = 0; i < cnt; ++i) {
        //     printf("%d  -->  %d\n", keys[i], ht.get(keys+i));
        // }

        return 0;
}
