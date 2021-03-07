#include <unify.h>

#include <vector>
#include <cstdint>
#include <algorithm>
#include <assert.h>
#include <set>

struct unified_entry {
    uintptr_t address;
    size_t size; // will not need this eventually
};

struct alloc_entry {
    unified_entry unif; 
    void* allocation;

    bool operator < (const alloc_entry& other) const {
        return unif.address < other.unif.address;
    }
};

static std::set<alloc_entry> alloc_list;
static std::vector<unified_entry> free_list;

extern "C" {

void* register_alloc(void* ptr, size_t bytes) {
    auto free = std::find_if(free_list.begin(), free_list.end(), [bytes](const auto& entry) -> bool {
        return entry.size >= bytes;
    });
    if (free != free_list.end()) {
        if (bytes == free->size) {
            alloc_list.insert({
                .unif = *free,
                .allocation = ptr
            });
            free_list.erase(free);
        } else {
            assert(free->size > bytes);
            alloc_list.insert({
                .unif = {
                    .address = free->address + bytes,
                    .size = free->size - bytes
                },
                .allocation = ptr,
            });
            free->size -= bytes; // if I ever sort the free_list by size, I should reinsert here
            static_assert(std::is_same<decltype(free_list), std::vector<unified_entry>>::value); 
        }
    } else {
        alloc_list.insert(alloc_list.end(), {
            .unif = {
                .address = alloc_list.empty() ? 0 : alloc_list.rbegin()->unif.address + alloc_list.rbegin()->unif.size,
                .size = bytes
            },
            .allocation = ptr
        });
        if (alloc_list.size() >= 2) 
            assert(alloc_list.rbegin()->unif.address > alloc_list.begin()->unif.address);
    }
    return ptr;
}

void* unregister_alloc(void* ptr) {
    auto entry = std::find_if(alloc_list.begin(), alloc_list.end(), [ptr](const auto& other) -> bool {
        return other.allocation == ptr;
    });
    assert(entry != alloc_list.end()); // crash signifies double or invalid free
    free_list.push_back(entry->unif);
    return ptr;
}

}


