#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <unify.h>

#include <sys/mman.h>

void check_int(size_t i) {
    assert(unify(deunify(i)) == i);
}

void check_int_range (size_t i, size_t size) {
    for (; i <= i + size; i++) {
        check_int(i);
    }
}

void check_ptr(void* p) {
    assert(deunify(unify(p)) == p);
}

void check_ptr_range(char* p, size_t size) {
    for (; p <= p + size; p++) {
        check_ptr(p);
    }
}

int main() {
    size_t size = 10 * sizeof(double);

    {
        void *a = malloc(size);
        memset(a, 0xdeadbeef, size);
        free((a));
    }

    char* block = malloc(size);
    char* zeroblock = calloc(1, size);

    printf("is memory the same? %d\n", memcmp(block, zeroblock, size) == 0);

    // size_t as_int = (size_t) unify(block);
    // double* block_1 = (double*) deunify(as_int + sizeof(double));
    // double* block_9 = (double*) deunify(as_int + 9*sizeof(double));
    // *block_1 = 9.6;
    // *block_9 = 88.8;
    // double* one_past_block = block_9 + 1;
    // size_t one_past_as_int = as_int + 10*sizeof(double);

    // check_int_range(as_int, size);
    // check_ptr_range(block, size);
    // check_int_range(unify(zeroblock), size);
    // check_ptr_range(zeroblock, size);

    free (block);
    free (zeroblock);
}

