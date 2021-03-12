#include <unify.h>

#include <vector>
#include <algorithm>
#include <iostream>
#include <mutex>

#include "unify_common.h"


static_assert(sizeof(uintptr_t) == 8);
static_assert(sizeof(char) == 1);

constexpr size_t ALIGNED_SIZE = UINT32_MAX;
constexpr size_t OFFSET_WIDTH = UINT32_WIDTH;

struct allocation {
    void* ptr;

    bool operator == (const allocation& other) {
        return ptr == other.ptr;
    }

    void* region_end() const {
        return static_cast<char*>(ptr) + ALIGNED_SIZE;
    }

    bool contains (void* other) const {
        return ptr <= other && static_cast<char*>(region_end()) > other;
    }

    bool operator == (void* other) const {
        return contains(other);
    }

    size_t offset_of (void* other) const {
        dbg_assert(this->contains(other));
        return static_cast<char*>(other) - static_cast<char*>(ptr);
    }
};

// SOA type thing
static std::vector<allocation>* alloc_list = nullptr;
static std::vector<bool>* free_list = nullptr;
static std::mutex* protect;
// would it be possible to use `nullptr` as free? 
// The vector<bool> memory overhead is small though
// (maybe for caching it is worth it)

run manage_vectors(
[](){
    unhook_scope guard{};
    protect = new std::mutex{};
    alloc_list = new std::vector<allocation>();
    free_list = new std::vector<bool>();
    alloc_list->push_back({nullptr});
    free_list->push_back(false);
}, 
[](){
    unhook_scope guard{};
    delete alloc_list;
    delete free_list;
    delete protect;
});

extern "C" {

void* register_alloc(void* ptr) {
    if (!alloc_list || !free_list)
        return ptr;

    unhook_scope guard{};
    std::lock_guard<std::mutex>{*protect};

    dbg_assert(ptr != nullptr);

    size_t bytes = malloc_usable_size(ptr);
    //std::cout << "`register_alloc(" << ptr << ", " << bytes << ")`" << std::endl;
    dbg_assert(bytes < UINT32_MAX);
    
    { // DEBUG
        auto again = std::find(alloc_list->begin(), alloc_list->end(), allocation{ptr});
        bool already_registered = !(again == alloc_list->end() || free_list->at(std::distance(alloc_list->begin(), again)));
        if (already_registered) {
            //std::cout << ptr << " fits the bucket between " << again->ptr << " and " << again->region_end() << std::endl;
        }
        //dbg_assert(!already_registered); // failing means nullptr right?
    }

    auto free = std::find(free_list->begin(), free_list->end(), true);
    if (free != free_list->end()) {
        size_t occ_idx = std::distance(free_list->begin(), free);
        alloc_list->at(occ_idx) = {ptr};
        *free = false;
    } else {
        alloc_list->push_back({ptr});
        free_list->push_back(false);
    }

    dbg_assert(alloc_list->size() == free_list->size());

    return ptr;
}

void* unregister_alloc(void* ptr) {
    if (!alloc_list || !free_list)
        return ptr;

    unhook_scope guard{};
    std::lock_guard<std::mutex>{*protect};
    
    dbg_assert(ptr != nullptr);
    
    auto alloc = std::find(alloc_list->begin(), alloc_list->end(), allocation{ptr});
    dbg_assert(alloc != alloc_list->end()); // double free (does not catch all)
    dbg_assert(alloc->offset_of(ptr) == 0);
    size_t occ_idx = std::distance(alloc_list->begin(), alloc);
    dbg_assert(free_list->at(occ_idx) == false);
    free_list->at(occ_idx) = true;

    // the below is not necessary but might reduce memory consumption a little bit 
    // (probably just hurts performance though)
    while (*free_list->rbegin()) {
        free_list->pop_back();
        alloc_list->pop_back();
    }

    return ptr;
}

// std::min_element could work here
static std::vector<allocation>::iterator closest_alloc_containing(char* v_addr) {
    size_t idx = alloc_list->size();
    size_t lowest_diff = SIZE_MAX;
    for (size_t i = 0; i < alloc_list->size(); i++) {
        size_t diff = v_addr >= static_cast<char*>(alloc_list->at(i).ptr) ? v_addr - static_cast<char*>(alloc_list->at(i).ptr) : SIZE_MAX;
        if (diff < lowest_diff) {
            lowest_diff = diff;
            idx = i;
            if (lowest_diff == 0)
                break;
        }
    }
    return alloc_list->begin() + idx;
}

uintptr_t unify(void* v_addr) {
    unhook_scope guard{};
    std::lock_guard<std::mutex>{*protect};

    printf("`unify(%p)`\n", v_addr);
    auto alloc = closest_alloc_containing(static_cast<char*>(v_addr));

    if (alloc == alloc_list->end())  { // this is an ICE
        dbg_assert(v_addr == nullptr); // is gonna fail
        return reinterpret_cast<uintptr_t>(v_addr);
    }

    uintptr_t occ_idx = std::distance(alloc_list->begin(), alloc) << OFFSET_WIDTH;
    occ_idx += alloc->offset_of(v_addr); // calculated again!! I should return a pair with idx and diff from `closest_alloc_containing`
    return occ_idx;
}

// I have to make sure to not call this on ICE's. They will break the system.
void* deunify(uintptr_t u_addr) {
    unhook_scope guard{};
    std::lock_guard<std::mutex>{*protect};

    return static_cast<char*>(alloc_list->at(u_addr >> OFFSET_WIDTH).ptr) + (u_addr & ALIGNED_SIZE);
}

void print_metadata() {
    unhook_scope guard{};
    // std::lock_guard<std::mutex>{*protect};

    std::cout << "----------------" << std::endl;
    for (uintptr_t idx = 0; idx < alloc_list->size(); idx++) {
        std::cout << "idx: " << idx << std::endl;
        std::cout << "occ_idx: " << (idx << OFFSET_WIDTH) << " (until " << (((idx + 1) << OFFSET_WIDTH) - 1) << ") " << std::endl;
        std::cout << "  allocation: " << alloc_list->at(idx).ptr << " (until " << alloc_list->at(idx).region_end() << ") " << std::endl;
        std::cout << "  free: " << free_list->at(idx) << std::endl;
        std::cout << "----------------" << std::endl;
    }
}

}
