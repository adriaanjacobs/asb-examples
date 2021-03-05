#include "hashtable.h"
#include "dyn_array.h"
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <unify.h>

typedef struct kv_pair {
    void* key;
    void* value;
} kv_pair_t;

struct hashtable {
    size_t max_collisions;
    dyn_array* buckets;
};

size_t hash(size_t val) {
    return val; // amazing hash function
}

size_t hashtable_most_collisions(hashtable* table) {
    assert(table != NULL);
    size_t max = 0;
    for (size_t buck = 0; buck < hashtable_bucket_count(table); buck ++) 
        max = fmaxl(max, dyn_array_size(*dyn_array_at(table->buckets, buck)));
    return max;
}

size_t hashtable_count_pairs(hashtable* table) {
    assert(table != NULL);
    size_t count = 0;
    for (size_t buck = 0; buck < hashtable_bucket_count(table); buck ++) 
        count += dyn_array_size(*dyn_array_at(table->buckets, buck));
    return count;
}

void hashtable_put(hashtable* table, void* key, void* value) {
    assert(table != NULL);
    if (hashtable_most_collisions(table) > table->max_collisions)  // arbitrary rehash point
        hashtable_rehash(table, 2 * hashtable_bucket_count(table));
    assert(hashtable_bucket_count(table) != 0);

    kv_pair_t* pair = malloc(sizeof(*pair));
    pair->key = key;
    pair->value = value;
    size_t idx = hash((key)) % hashtable_bucket_count(table);
    dyn_array_add(*dyn_array_at(table->buckets, idx), pair);
}

hashtable* hashtable_alloc() {
    return malloc(sizeof(hashtable));
}

void hashtable_init(hashtable* table, size_t max_collisions, size_t initial_capacity) {
    assert(table != NULL);
    table->buckets = dyn_array_alloc();
    dyn_array_init(table->buckets);
    dyn_array_reserve(table->buckets, initial_capacity);
    assert(hashtable_bucket_count(table) == initial_capacity);
    for (size_t i = 0; i < hashtable_bucket_count(table); i++) {
        *dyn_array_at(table->buckets, i) = dyn_array_alloc();
        dyn_array_init(*dyn_array_at(table->buckets, i));
    }

    table->max_collisions = max_collisions;
}

void hashtable_rehash(hashtable* table, size_t new_capacity) {
    assert(table != NULL);
    hashtable newtable;
    hashtable_init(&newtable, table->max_collisions, new_capacity);
    assert(hashtable_bucket_count(&newtable) == new_capacity);

    for (size_t buck = 0; buck < hashtable_bucket_count(table); buck++) {
        dyn_array* bucket = *dyn_array_at(table->buckets, buck);
        for (size_t p_i = 0; p_i < dyn_array_size(bucket); p_i++) {
            kv_pair_t* pair = *dyn_array_at(bucket, p_i);
            hashtable_put(&newtable, pair->key, pair->value);
        }
    }

    hashtable_destruct(table);
    *table = newtable;
}

size_t hashtable_bucket_count(hashtable* table) {
    assert(table != NULL);
    return dyn_array_size(table->buckets);
}

void hashtable_foreach(hashtable* table, void (*func)(void* key, void* value)) {
    assert(table != NULL);
    for (size_t buck = 0; buck < hashtable_bucket_count(table); buck++) {
        dyn_array* bucket = *dyn_array_at(table->buckets, buck);
        for (size_t p_i = 0; p_i < dyn_array_size(bucket); p_i++) {
            kv_pair_t* pair = *dyn_array_at(bucket, p_i);
            func(pair->key, pair->value);
        }
    }
}

void hashtable_print(hashtable* table) {
    assert(table != NULL);
    for (size_t buck = 0; buck < hashtable_bucket_count(table); buck++) {
        dyn_array* bucket = *dyn_array_at(table->buckets, buck);
        for (size_t p_i = 0; p_i < dyn_array_size(bucket); p_i++) {
            kv_pair_t* pair = *dyn_array_at(bucket, p_i);
            printf("%p --> %p\n", pair->key, pair->value);
        }
    }
}

void hashtable_destruct(hashtable* table) {
    assert(table != NULL);
    for (size_t buck = 0; buck < dyn_array_size(table->buckets); buck++) {
        dyn_array* bucket = *dyn_array_at(table->buckets, buck);
        for (size_t pair = 0; pair < dyn_array_size(bucket); pair++)
            free(*dyn_array_at(bucket, pair));
        dyn_array_destruct(*dyn_array_at(table->buckets, buck));
    }

    dyn_array_destruct(table->buckets);
}

void hashtable_free(hashtable* table) {
    free(table);
}
