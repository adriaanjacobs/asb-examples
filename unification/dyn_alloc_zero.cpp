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

#include <unify.h>
#include "unify_common.h"

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

#ifdef __cplusplus
extern "C" {
#endif

static __thread bool hook_active = false;

void hook_all() {
    hook_active = true;
}

void unhook_all() {
    hook_active = false;
}

bool hook_status() {
    return hook_active;
}

static void* (*real_malloc)(size_t) = NULL;
static void (*real_free)(void*) = NULL;
static void* (*real_memalign)(size_t, size_t) = NULL;

inline void init_real_functions() {
    if (UNLIKELY(!real_malloc)) {
        assert(hook_status() == false);
        real_malloc = (void* (*)(size_t)) dlsym(RTLD_NEXT, "malloc");
        real_free = (void (*)(void*)) dlsym(RTLD_NEXT, "free");
        real_memalign = (void* (*)(size_t, size_t)) dlsym(RTLD_NEXT, "memalign");

        atexit(unhook_all);
        hook_all();
    }
}

void* malloc_zero(size_t bytes) {
    void* ret = real_malloc(bytes);
    memset(ret, 0, bytes);
    return ret;
}

// Allocate a block of size bytes. See Basic Allocation.
void* malloc(size_t bytes) {
    init_real_functions();
    // dl_sym does not call malloc
    if (!hook_active)  
        return malloc_zero(bytes);
    unhook_scope guard{};

    void* ret = malloc_zero(bytes); 
    printf("My malloc called for %lu bytes, returning %p \n", bytes, ret);
    register_alloc(ret, bytes);

    return ret;
}

// Free a block previously allocated by malloc. See Freeing after Malloc.
void free (void *addr) {
    init_real_functions();
    if (!hook_active)  {
        real_free(addr);
        return;
    }
    unhook_scope guard{};

    real_free(addr); // does not necessarily need to be protected
    printf("My free called on %p \n", addr);
    unregister_alloc(addr);
}

void* memalign_zero(size_t alignment, size_t size) {
    void* ret = real_memalign(alignment, size);
    memset(ret, 0, size);
    return ret;
}

// Allocate a block of size bytes, starting on an address that is a multiple of boundary. See Aligned Memory Blocks.
void *memalign (size_t alignment, size_t size) {
    init_real_functions();
    if (!hook_active)  
        return memalign_zero(alignment, size);
    unhook_scope guard{};

    void* ret = memalign_zero(alignment, size);
    register_alloc(ret, size); 

    return ret;
}

// Make a block previously allocated by malloc larger or smaller, possibly by copying it to a new location. See Changing Block Size.
void *realloc (void *addr, size_t size) {
    // Since I can't know the size of the block without using 
    // implementation-specific tactics, I can only use 
    // the allocator unification implementation here
    //
    // Also, I haven't thought about how to actually deal with allocation resizing yet.
    void* newblock = malloc(size);
    unhook_scope guard{};
    printf("realloc(%p, %lu) returns %p\n", addr, size, newblock);
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
    if (!real_malloc)
        return NULL; // Do not ask me why, but dlsym is totally fine with a nullptr here

    // Since dlsym calls `calloc` when linked with pthread
    // I cannot let this guy go to libc
    printf("Calloc called. dlsym maybe? \n");
    return malloc(count * eltsize);
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


#ifdef __cplusplus
}
#endif
