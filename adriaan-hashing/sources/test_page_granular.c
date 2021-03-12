#include "unify.h"
#include <malloc.h>
#include <sys/mman.h>



int main() {

    for ( int i = 0; i < 10; i++) {
        printf("malloc returns: %p \n", malloc(i * 351535));
    }

    mmap(NULL, 0, 0, 0, 0, 0);


    return 0;
}


