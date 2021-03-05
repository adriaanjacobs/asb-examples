#include "hashtable.h"
#include <malloc.h>

void print_bucket(void* key, void* value) {
    printf("About to print \n");
    printf("%d --> %d\n", key, value);
}

int main() {
    hashtable* table = hashtable_alloc();
    hashtable_init(table, 5, 10);

    const size_t count = 9999;
    int keys[count];
    int values[count];
    for (size_t i = 0; i < count; i++) {
        keys[i] = i*2;
        values[i] = i*3;
    }

    for (size_t i = 0; i < count; i++) {
        hashtable_put(table, keys[i], values[i]);
    }

    hashtable_foreach(table, print_bucket);

    hashtable_destruct(table);
    hashtable_free(table);
}