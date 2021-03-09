#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>

int main() {
    size_t size = 10 * sizeof(double);

    {
        void *a = malloc(size);
        memset(a, 0xdeadbeef, size);
        free(a);
    }

    //char* block = malloc(size);
    //char* zeroblock = calloc(1, size);

    //printf("is memory the same? %d\n", memcmp(block, zeroblock, size) == 0);

    // size_t as_int = (size_t) block;
    // double* block_1 = (double*) (as_int + sizeof(double));
    // double* block_9 = (double*) (as_int + 9*sizeof(double));
    // *block_1 = 9.6;
    // *block_9 = 88.8;
    // double* one_past_block = block_9 + 1;
    // size_t one_past_as_int = as_int + 10*sizeof(double);
    
    // assert((size_t)one_past_block == one_past_as_int);
    // assert((double*)one_past_as_int == one_past_block);

}

