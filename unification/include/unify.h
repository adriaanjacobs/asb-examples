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
Problem I'm ignoring for now: the deallocation of the std::vectors definitely calls the custom
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
*/

void* register_alloc(void* ptr, size_t bytes);

void* unregister_alloc(void* ptr);

uintptr_t unify(void* v_addr);

void* deunify(uintptr_t u_addr);

// I don't think every allocation algorithm implementation exposes this. 
// Maybe only the aligned_up size? Would not be a problem as long as it's the 
// aligned_up size used by the allocator implementation, not just any
// that I could use for it.
size_t size_of_alloc(void* ptr);

void print_metadata();

#ifdef __cplusplus
}
#endif

// __NO_INLINE__ can be used to detect if we need the boolean


// #define __UNIFICATION_ZERO_STACK_FRAME() \
//     do {
//         register char * const basepointer  asm("ebp");
//         register char * const stackpointer asm("esp");
//             memset(stackpointer, 0, basepointer - stackpointer - sizeof(long));
//     } while (false)

#endif //_UNIFY_H_GUARD_PLEASE_DO_NOT_BE_DEFINED_ANYWHERE_ALREADY_THAT_WOULD_BE_A_SHITTY_ERROR_TO_FIX_

