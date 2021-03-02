#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <unistd.h>

typedef struct hashtable hashtable;


hashtable* hashtable_init(size_t max_collisions, size_t initial_capacity);

void hashtable_put(hashtable* table, void* key, void* value);

void hashtable_get(hashtable* table, void* key);

void hashtable_rehash(hashtable* table, size_t new_capacity);

size_t hashtable_bucket_count(hashtable* table);

void hashtable_print(hashtable* table);

void hashtable_destroy(hashtable* table);

#endif