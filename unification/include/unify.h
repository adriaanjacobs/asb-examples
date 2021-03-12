#ifndef _UNIFY_H_GUARD_PLEASE_DO_NOT_BE_DEFINED_ANYWHERE_ALREADY_THAT_WOULD_BE_A_SHITTY_ERROR_TO_FIX_
#define _UNIFY_H_GUARD_PLEASE_DO_NOT_BE_DEFINED_ANYWHERE_ALREADY_THAT_WOULD_BE_A_SHITTY_ERROR_TO_FIX_

#include <stdint.h>
#include <stddef.h>

// I should maybe make sure these symbols are more unique.
// Their implementations get preloaded (statically linked into the malloc overloads) 
// and I don't want to intercept any calls to them.

#ifdef __cplusplus
extern "C" {
#endif

/*
    Problem: the deallocation of the std::vectors definitely calls the custom
    free() (as such they will be registered), so their allocation should too.  Registering their 
    allocation however will make them call malloc() themselves, so it would be up to the 
    std::vector implementation when it will be able to insert an element without reallocating. 
    (so probably not a huge problem). However, proposed fix(es):
        1. Not calling the custom free() by implementing a `deinit()` function that unhooks all 
            malloc()- related functions (or clears the vectors) and using it with `atexit` (is called before destruction
            of global objects). This would enforce that all of the below functions
            can never use the custom malloc()-functions, so unify/deunify should never dynamically
            allocate and the (un)register functions should never be called outside of the custom 
            functions (unless protected with their hooks, which should then be publicly controllable)
        2. Make the functions below intrinsicly safe by letting them control whether the malloc() functions
            are hooked or not. This has the advantage that I can call them anywhere, anytime.

    Since the reallocations inside the vector may never be seen by the registration functions 
    (to allow for easy registration of stack allocations), neither may their allocation nor their
    deallocation. I therefore construct them myself in a controlled way (without registration) 
    and also destruct them without registration. I think this fully works.
*/

void* register_page(void* ptr, size_t amount);

void* unregister_page(void* ptr, size_t amount);

void* register_alloc(void* ptr);

void* unregister_alloc(void* ptr);

uintptr_t unify(void* v_addr);

void* deunify(uintptr_t u_addr);

void print_metadata();

#ifdef __cplusplus
}
#endif

#endif //_UNIFY_H_GUARD_PLEASE_DO_NOT_BE_DEFINED_ANYWHERE_ALREADY_THAT_WOULD_BE_A_SHITTY_ERROR_TO_FIX_

