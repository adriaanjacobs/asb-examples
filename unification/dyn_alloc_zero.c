#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <stdio.h>
#include <dlfcn.h>
#include <memory.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

#include <unify.h>


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


// Allocate a block of size bytes. See Basic Allocation.
void* malloc(size_t bytes) {
    // dl_sym does not call malloc
    GENERATE_DLSYM(void*, malloc, size_t);
    static __thread bool no_hook = false;
    if (no_hook) 
        return real_malloc(bytes);
    no_hook = true;

    void* ret = real_malloc(bytes); 
    register_alloc(ret, bytes);
    memset(ret, 0, bytes);

    no_hook = false;
    return ret;
}

// Free a block previously allocated by malloc. See Freeing after Malloc.
void free (void *addr) {
    GENERATE_DLSYM(void, free, void*);
    static __thread bool no_hook = false;
    if (no_hook) {
        real_free(addr);
        return;
    }
    no_hook = true;

    unregister_alloc(addr);
    real_free(addr); // does not necessarily need to be protected
    
    no_hook = false;
}

// Make a block previously allocated by malloc larger or smaller, possibly by copying it to a new location. See Changing Block Size.
void *realloc (void *addr, size_t size) {
    // Since I can't know the size of the block without using 
    // implementation-specific tactics, I can only use 
    // the allocator unification implementation here
    //
    // Also, I haven't thought about how to actually deal with allocation resizing yet.
    void* newblock = malloc(size);
    memcpy(newblock, addr, fmin(size, size_of_alloc(addr)));
    free(addr);
    return newblock;
}

// Change the size of a block previously allocated by malloc to nmemb * size bytes as with realloc. See Changing Block Size.
void *reallocarray (void *ptr, size_t nmemb, size_t size) {
    return realloc(ptr, nmemb * size);
}

// Allocate a block of count * eltsize bytes using malloc, and set its contents to zero. See Allocating Cleared Space.
void *calloc (size_t count, size_t eltsize) {
    // Since dlsym calls `calloc` when linked with pthread
    // I cannot let this guy go to libc
    return malloc(count * eltsize);
}

// Allocate a block of size bytes, starting on an address that is a multiple of boundary. See Aligned Memory Blocks.
void *memalign (size_t alignment, size_t size) {
    GENERATE_DLSYM(size_t, memalign, size_t, size_t);
    static __thread bool no_hook = false;
    if (no_hook) 
        return real_memalign(alignment, size);
    no_hook = true;

    void* ret = real_memalign(alignment, size); 
    register_alloc(ret, size);
    memset(ret, 0, size);

    no_hook = false;
    return ret;
}

// Allocate a block of size bytes, starting on a page boundary. See Aligned Memory Blocks.
void *valloc (size_t size) {
    return memalign(getpagesize(), size);
}

// Allocate a block of size bytes, starting on an address that is a multiple of alignment. See Aligned Memory Blocks.
void *aligned_alloc (size_t alignment, size_t size) {
    if (size % alignment != 0) { // don't know why I do this, the glibc implementation doesn't even check for this AFAIK
        errno = EINVAL;
        return NULL;
    }
    return memalign(alignment, size);
}

// Allocate a block of size bytes, starting on an address that is a multiple of alignment. See Aligned Memory Blocks.
int posix_memalign (void **memptr, size_t alignment, size_t size) {
    if (alignment % sizeof(void*) == 0 && powerof2(alignment/sizeof(void*))) {
        *memptr = memalign(alignment, size);
        if (*memptr)
            return 0;
        return errno;
    }
    *memptr = NULL;
    return EINVAL;
}

