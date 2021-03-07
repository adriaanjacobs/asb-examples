#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <memory.h>

#include <unify.h>

#define LIKELY(expr) __builtin_expect(!!(expr), 1)
#define UNLIKELY(expr) __builtin_expect(!!(expr), 0)

static void* malloc_impl(size_t bytes) {
    static __thread bool no_hook = 0;
    static void* (*real_malloc)(size_t) = NULL;
    if (UNLIKELY(!real_malloc)) 
        real_malloc = (void * (*)(size_t))dlsym(RTLD_NEXT, "malloc");

    if (no_hook) 
        return real_malloc(bytes);

    no_hook = true;
    printf("malloc call %zu bytes\n", bytes);
    void* ret = real_malloc(bytes); 
    no_hook = false; 
    return ret; 
}


void* malloc(size_t bytes) {
    void* ret = malloc_impl(bytes);
    memset(ret, 0, bytes);
    return ret;
}


