#include "hashtable.h"


int main() {

    hashtable* table = hashtable_init(5, 10);

    const size_t count = 100;
    int keys[count];
    int values[count];

    for (size_t i = 0; i < count; i++) {
        hashtable_put(table, keys + i, values + i);
    }

    hashtable_print(table);

    hashtable_destroy(table);
}