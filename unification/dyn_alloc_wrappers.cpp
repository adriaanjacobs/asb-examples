


#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <memory.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>
#include <malloc.h>
#include <atomic>

#include <unify.h>
#include "unify_common.h"
#include "temporary_allocator.h"

#define LIKELY(expr) __builtin_expect(!!(expr), 1)
#define UNLIKELY(expr) __builtin_expect(!!(expr), 0)

#define TO_STRING(s) TO_STRING_WEAK(s)
#define TO_STRING_WEAK(s) #s

#define GENERATE_DLSYM(rettype, funcname, ...) \
    static rettype (*real_##funcname)(__VA_ARGS__) = NULL;  \
    do {                                                            \
        if (UNLIKELY(!real_##funcname))                         \
            real_##funcname = (rettype (*)(__VA_ARGS__)) dlsym(RTLD_NEXT, TO_STRING(funcname)); \
    } while (false)

#define powerof2(x) ((x != 0) && ((x & (x - 1)) == 0))

static __thread std::atomic_bool hook_active {false};
static __thread std::atomic_bool hook_override_active {false};



void override_hook_all() {
    hook_override_active = true;
}

void override_unhook_all() {
    hook_override_active = false;
}

void hook_all() {
    hook_active = true;
}

void unhook_all() {
    hook_active = false;
}

bool hook_status() {
    return hook_active && hook_override_active;
}

run hook_em {
    [](){
        atexit(unhook_all);
        hook_all();
        override_hook_all();
    }, nullptr
};

/*
    Other means of allocation I should still cover:
        1. mmap family for custom application-specific allocators 
        2. sbrk family 
        3. alloca 
*/

extern "C" {

void* __real_malloc(size_t bytes);
void __real_free(void* ptr);
void* __real_memalign(size_t alignment, size_t bytes);

void* malloc_zero(size_t bytes) {
    void* ret = __real_malloc(bytes);
    memset(ret, 0, bytes);
    return ret;
}

// Allocate a block of size bytes. See Basic Allocation.
void* __wrap_malloc(size_t bytes) {
    if (!hook_status())
        return malloc_zero(bytes);
    unhook_scope guard{};

    void* ret = malloc_zero(bytes); 
    //printf("My malloc called for %lu bytes, returning %p \n", bytes, ret);
    if (ret)
        register_alloc(ret);

    return ret;
}

// Free a block previously allocated by malloc. See Freeing after Malloc.
void __wrap_free (void *addr) {
    if (!hook_status())  {
        __real_free(addr);
        return;
    }
    unhook_scope guard{};

    __real_free(addr); // does not necessarily need to be protected
    //printf("My free called on %p \n", addr);
    if (addr)
        unregister_alloc(addr);
}

void* memalign_zero(size_t alignment, size_t size) {
    void* ret = __real_memalign(alignment, size);
    memset(ret, 0, size);
    return ret;
}

// Allocate a block of size bytes, starting on an address that is a multiple of boundary. See Aligned Memory Blocks.
void* __wrap_memalign (size_t alignment, size_t size) {
    if (!hook_status())  
        return memalign_zero(alignment, size);
    unhook_scope guard{};

    void* ret = memalign_zero(alignment, size);
    if (ret)
        register_alloc(ret); 

    return ret;
}

// Make a block previously allocated by malloc larger or smaller, possibly by copying it to a new location. See Changing Block Size.
void* __wrap_realloc (void *addr, size_t size) {
    // Since I can't know the size of the block without using 
    // implementation-specific tactics, I can only use 
    // the allocator unification implementation here
    //
    // Also, I haven't thought about how to actually deal with allocation resizing yet.
    void* newblock = malloc(size);
    unhook_scope guard{};
    //printf("realloc(%p, %lu) returns %p\n", addr, size, newblock);
    memcpy(newblock, addr, fmin(size, malloc_usable_size(addr)));
    free(addr);
    return newblock;
}

// Change the size of a block previously allocated by malloc to nmemb * size bytes as with realloc. See Changing Block Size.
void* __wrap_reallocarray (void *ptr, size_t nmemb, size_t size) {
    return realloc(ptr, nmemb * size);
}

// Allocate a block of count * eltsize bytes using malloc, and set its contents to zero. See Allocating Cleared Space.
void* __wrap_calloc (size_t count, size_t eltsize) {
    return malloc(count * eltsize);
}

// Allocate a block of size bytes, starting on a page boundary. See Aligned Memory Blocks.
void* __wrap_valloc (size_t size) {
    return memalign(getpagesize(), size);
}

// Allocate a block of size bytes, starting on an address that is a multiple of alignment. See Aligned Memory Blocks.
void* __wrap_aligned_alloc (size_t alignment, size_t size) {
    if (size % alignment != 0) { // don't know why I do this, the glibc implementation doesn't even check for this AFAIK
        errno = EINVAL;
        return NULL;
    }
    return memalign(alignment, size);
}

// Allocate a block of size bytes, starting on an address that is a multiple of alignment. See Aligned Memory Blocks.
int __wrap_posix_memalign (void **memptr, size_t alignment, size_t size) {
    if (alignment % sizeof(void*) == 0 && powerof2(alignment/sizeof(void*))) {
        *memptr = memalign(alignment, size);
        if (*memptr)
            return 0;
        return errno;
    }
    *memptr = NULL;
    return EINVAL;
}


}
