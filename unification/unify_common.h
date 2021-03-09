#include <stdlib.h>

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

struct dummy_initialization {
    dummy_initialization() {
        // atexit runs before global object destruction
        atexit(unhook_all);
    }
};

extern dummy_initialization init;