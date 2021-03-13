#ifndef MMAP_VECTOR_DEFINED_PLEASE_DONT_USE_K_THX_BYE
#define MMAP_VECTOR_DEFINED_PLEASE_DONT_USE_K_THX_BYE

#include <sys/mman.h>
#include <unistd.h>
#include <memory.h>
#include <assert.h>
#include <functional>

template<typename T>
class mmap_vector {
    static constexpr size_t BACKED_AMOUNT = 420;

    struct element {
        T user_thing;
        bool free;
    };
    element backing_store[BACKED_AMOUNT];

public:
    mmap_vector() {
        memset(backing_store, 0, BACKED_AMOUNT * sizeof(element));
    }

    T& at(size_t idx) {
        assert(idx < BACKED_AMOUNT);
        return *reinterpret_cast<T*>(backing_store + idx + offsetof(element, user_thing));
    }

    int find_if(std::function<bool(const T&)> func) {
        for (size_t i = 0; i < BACKED_AMOUNT; i++) {
            if(func(backing_store[i].user_thing))
                return i;
        }
        return -1;
    }

    void add(const T& thing) {
        for (size_t i = 0; i < BACKED_AMOUNT; i++) {
            if (backing_store[i].free) {
                backing_store[i].user_thing = thing;
                return;
            }
        }
        assert(false && "Tried to allocate more than BACKED_AMOUNT times");
    }

    void remove_at(size_t idx) {
        at(idx);
        backing_store[idx].free = true;
    }
};


#endif