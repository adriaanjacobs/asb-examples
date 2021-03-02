#include "dyn_array.h"
#include <assert.h>
#include <memory.h>
#include <malloc.h>

struct dyn_array {
    void** array;
    size_t size;
    size_t capacity;
};

size_t dyn_array_size(dyn_array* darr) {
    assert(darr != NULL);
    return darr->size;
}

dyn_array* dyn_array_alloc() {
    return calloc(sizeof(dyn_array), 1);
}

void dyn_array_init(dyn_array* darr) {
    assert(darr != NULL);
    memset(darr, 0, sizeof(*darr));
}

void dyn_array_reserve(dyn_array* darr, size_t size) {
    assert(darr != NULL);
    assert(size > darr->size); // downsizing not handled

    for (size_t i = darr->size; i < size; i++) 
        dyn_array_add(darr, NULL);
    assert(size == darr->size);
}

void dyn_array_add(dyn_array* darr, void* val) {
    assert(darr != NULL);
    if(darr->size >= darr->capacity) {
        darr->capacity = (darr->capacity * 2) + 1;
        assert(darr->capacity != 0);
        void** newdarr = malloc(darr->capacity * sizeof(*darr->array));
        memcpy(newdarr, darr->array, darr->size * sizeof(*newdarr));
        free(darr->array);
        darr->array = newdarr;
    }
    assert(darr->size < darr->capacity);
    
    darr->array[darr->size++] = val;
}

void** dyn_array_at(dyn_array* darr, size_t idx) {
    assert(darr != NULL);
    assert(idx < darr->size);
    return darr->array + idx;
}

void dyn_array_destruct(dyn_array* darr) {
    assert(darr != NULL);
    free(darr->array);
    memset(darr, 0, sizeof(*darr));
}

void dyn_array_free(dyn_array* darr) {
    free(darr);
}
