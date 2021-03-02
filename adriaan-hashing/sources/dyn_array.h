#ifndef _DYN_ARRAY_H_
#define _DYN_ARRAY_H_

#include <unistd.h>

typedef struct dyn_array dyn_array;

dyn_array* dyn_array_alloc();

void dyn_array_init(dyn_array* darr);

size_t dyn_array_size(dyn_array* darr);

void dyn_array_reserve(dyn_array* darr, size_t size);

void dyn_array_add(dyn_array* darr, void* val);

void** dyn_array_at(dyn_array* darr, size_t idx);

void dyn_array_destruct(dyn_array* darr);

void dyn_array_free(dyn_array* darr);

#endif
