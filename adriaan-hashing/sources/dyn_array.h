#ifndef _DYN_ARRAY_H_
#define _DYN_ARRAY_H_

#include <unistd.h>

typedef struct dyn_array dyn_array;


size_t dyn_array_get_size(dyn_array* darr);

dyn_array* dyn_array_init();

void dyn_array_reserve(dyn_array* darr, size_t size);

void dyn_array_add(dyn_array* darr, void* val);

void** dyn_array_at(dyn_array* darr, size_t idx);

void dyn_array_destroy(dyn_array* darr);

#endif