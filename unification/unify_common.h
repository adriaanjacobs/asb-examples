#include <stdlib.h>
#include <functional>
#include <malloc.h>

#if 1
    #define BOOST_STACKTRACE_USE_ADDR2LINE
    #include <boost/stacktrace.hpp>
    #include <iostream>
#endif

#include "dyn_alloc_zero.h"

// std::cout << boost::stacktrace::stacktrace() << std::endl; 
// //print_metadata();      
#define dbg_assert(expr) \
    do {                        \
        if (!!!(expr)) {        \
            override_unhook_all();           \
            printf("%s:%d: Assertion `" #expr "` failed. \n", __FILE__, __LINE__);  \
            printf("Metadata on fail: \n");    \
            std::cout << boost::stacktrace::stacktrace();  \
            printf("ASB: Exited with error. \n");  \
            fflush(stdout);  \
            exit(-1);  \
        }                       \
    } while (false)


// helper guard class
struct unhook_scope {
    bool was_hooked;
    unhook_scope() {
        was_hooked = hook_status();
        unhook_all();
    }

    ~unhook_scope() {
        if (was_hooked) 
            hook_all();
        else {
            dbg_assert(hook_status() == false && "`hooked` boolean must be thread-local!");
        }
    }
};

struct run {
    std::function<void()> destr;
    run(std::function<void()> constr, std::function<void()> destr) {
        if (constr) 
            constr();
        this->destr = destr;
    }

    ~run() {
        if (destr)
            destr();
    }
};

