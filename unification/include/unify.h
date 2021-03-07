#ifndef _UNIFY_H_GUARD_PLEASE_DO_NOT_BE_DEFINED_ANYWHERE_ALREADY_THAT_WOULD_BE_A_SHITTY_ERROR_TO_FIX_
#define _UNIFY_H_GUARD_PLEASE_DO_NOT_BE_DEFINED_ANYWHERE_ALREADY_THAT_WOULD_BE_A_SHITTY_ERROR_TO_FIX_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
    #define EXTERNC extern "C"
#else
    #define EXTERNC
#endif

EXTERNC void* register_alloc(void* ptr, size_t bytes);

EXTERNC void* unregister_alloc(void* ptr);

EXTERNC uintptr_t unify(void* v_addr);

EXTERNC void* deunify(uintptr_t u_addr);

#undef EXTERNC

// __NO_INLINE__ can be used to detect if we need the boolean


// #define __UNIFICATION_ZERO_STACK_FRAME() \
//     do {
//         register char * const basepointer  asm("ebp");
//         register char * const stackpointer asm("esp");
//             memset(stackpointer, 0, basepointer - stackpointer - sizeof(long));
//     } while (false)

#endif //_UNIFY_H_GUARD_PLEASE_DO_NOT_BE_DEFINED_ANYWHERE_ALREADY_THAT_WOULD_BE_A_SHITTY_ERROR_TO_FIX_

