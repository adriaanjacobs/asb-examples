#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <unify.h>

void check_int(size_t i) {
    assert(unify(deunify(i)) == i);
}

void check_int_range (size_t occ_idx, size_t size) {
    printf("i: %lu, size: %lu \n", occ_idx, size);
    for (size_t i = occ_idx; i <= occ_idx + size--; i++) {
        check_int(i);
    }
}

void check_ptr(void* p) {
    assert(deunify(unify(p)) == p);
}

void check_ptr_range(char* p, size_t size) {
    for (char* incr = p; incr <= p + size; incr++) {
        check_ptr(incr);
    }
}

int main() {
    size_t size = 10 * sizeof(double);

    {
        void *a = malloc(size);
        memset(a, 0xdeadbeef, size);
        free((a));
    }

    print_metadata();

    char* block = malloc(size);
    char* zeroblock = calloc(1, size);

    printf("is memory the same? %d\n", memcmp(block, zeroblock, size) == 0);

    //print_metadata();

    printf("The unified NULL value: %lu \n", unify(NULL));

    size_t as_int = (size_t) unify(block);

    //void* test = realloc(NULL, 32);
    // double* block_1 = (double*) deunify(as_int + sizeof(double));
    // double* block_9 = (double*) deunify(as_int + 9*sizeof(double));
    // *block_1 = 9.6;
    // *block_9 = 88.8;
    // double* one_past_block = block_9 + 1;
    // size_t one_past_as_int = as_int + 10*sizeof(double);

    //check_int_range(as_int, 0);
    // check_ptr_range(block, size);
    // check_int_range(unify(zeroblock), size);
    // check_ptr_range(zeroblock, size);

    //free (block);
    //free (zeroblock);
}

