#pragma once

#ifndef TEMPORARY_ALLOCATOR_DONT_USE_THX
#define TEMPORARY_ALLOCATOR_DONT_USE_THX

#include <unistd.h>
#include "mmap_vector.h"

class temporary_allocator {

    struct alloc_data {
        void* base;
        size_t bytes;
    };

    mmap_vector<alloc_data> metadata;

public:
    void* temp_alloc(size_t bytes);
    void temp_free(void* ptr);
    void* temp_memalign(size_t alignment, size_t size);
};



#endif