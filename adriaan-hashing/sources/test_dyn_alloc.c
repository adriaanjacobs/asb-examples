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

    
    char* block = malloc(size);
    char* zeroblock = calloc(1, size);

   printf("is memory the same? %d\n", memcmp(block, zeroblock, size) == 0);
}

