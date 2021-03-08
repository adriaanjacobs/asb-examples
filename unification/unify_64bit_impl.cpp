#include <unify.h>

#include <vector>
#include <algorithm>
#include <assert.h>

static_assert(sizeof(uintptr_t) == 8);
static_assert(sizeof(char) == 1);

constexpr size_t ALIGNED_SIZE = UINT32_MAX;
constexpr size_t OFFSET_WIDTH = UINT32_WIDTH;

// SOA type thing
static std::vector<void*> alloc_list;
static std::vector<bool> free_list;
// would it be possible to use `nullptr` as free? That's assuming I never map nullptr
// I should query for patterns then like 
//          ((int)ptr) == 0 
//          (void*)some_int == nullptr
// extra implementation complexity is likely not worth the small vector<bool> memory overhead 
// (maybe for caching it is worth it)

extern "C" {

void* register_alloc(void* ptr, size_t bytes) {
    assert(bytes <= UINT32_MAX); 
    
    { // DEBUG
        auto again = std::find(alloc_list.begin(), alloc_list.end(), ptr);
        assert(again == alloc_list.end());
    }

    auto free = std::find(free_list.begin(), free_list.end(), true);
    if (free != free_list.end()) {
        size_t occ_idx = std::distance(free_list.begin(), free);
        alloc_list[occ_idx] = ptr;
        *free = false;
    } else {
        alloc_list.push_back(ptr);
        free_list.push_back(false);
    }

    assert(alloc_list.size() == free_list.size());
    return ptr;
}

void* unregister_alloc(void* ptr) {
    auto alloc = std::find(alloc_list.begin(), alloc_list.end(), ptr);
    assert(alloc != alloc_list.end());
    size_t occ_idx = std::distance(alloc_list.begin(), alloc);
    free_list[occ_idx] = true;
    return ptr;
}

uintptr_t unify(void* v_addr) {
    auto alloc = std::find_if(alloc_list.begin(), alloc_list.end(), [v_addr](void* ptr) {
        // //This is not safe C. It will work though (I think) in flat memory model archs
        return v_addr >= ptr && v_addr < (static_cast<char*>(ptr) + ALIGNED_SIZE); 
    });
    assert(alloc != alloc_list.end());

    uintptr_t occ_idx = std::distance(alloc_list.begin(), alloc) << OFFSET_WIDTH;
    occ_idx += (static_cast<char*>(v_addr) - static_cast<char*>(*alloc));
    return occ_idx;
}

void* deunify(uintptr_t u_addr) {
    return static_cast<char*>(alloc_list.at((u_addr >> OFFSET_WIDTH))) + (u_addr & ALIGNED_SIZE);
}


}
