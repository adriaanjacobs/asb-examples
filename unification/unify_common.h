#include <stdlib.h>
#include <functional>

#include "dyn_alloc_zero.h"

// helper guard class
struct unhook_scope {
    unhook_scope() {
        unhook_all();
    }

    ~unhook_scope() {
        hook_all();
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