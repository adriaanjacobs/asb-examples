#ifndef _UNIFY_H_GUARD_PLEASE_DO_NOT_BE_DEFINED_ANYWHERE_ALREADY_THAT_WOULD_BE_A_SHITTY_ERROR_TO_FIX_
#define _UNIFY_H_GUARD_PLEASE_DO_NOT_BE_DEFINED_ANYWHERE_ALREADY_THAT_WOULD_BE_A_SHITTY_ERROR_TO_FIX_

#include <stdint.h>
#include <stddef.h>

// I should maybe make sure these symbols are more unique.
// They get preloaded and I don't want to intercept any calls to them.

#ifdef __cplusplus
extern "C" {
#endif

void* register_alloc(void* ptr, size_t bytes);

void* unregister_alloc(void* ptr);

uintptr_t unify(void* v_addr);

void* deunify(uintptr_t u_addr);

// I don't think every allocation algorithm implementation exposes this. 
// Maybe only the aligned_up size? Would not be a problem
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

