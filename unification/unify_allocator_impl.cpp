#include <unify.h>

#include <vector>
#include <cstdint>
#include <algorithm>
#include <iostream>

#include "unify_common.h"

static_assert(sizeof(char) == 1);

struct alloc_entry {
    void* allocation;
    size_t size; // 1 byte bigger than the allocated size

    bool operator == (void* other) const {
        return allocation == other;
    }
};

// again, SOA thing
static std::vector<alloc_entry>* alloc_list = nullptr;
static std::vector<bool>* free_list = nullptr;

extern "C" {

static void* force_register_alloc(void* ptr, size_t bytes) {
    if (!alloc_list || !free_list)
        return ptr;
    unhook_scope guard{};
    printf("`register_alloc(%p, %lu)`;\n", ptr, bytes);

    /*
        In strict mode, pointers can point to one past an array
        I therefore assume this is also a common target of 
        integer conversions and include it here.
        In relaxed mode, pointers may take on any value as long
        as they're not dereferenced. Typical code operates in 
        relaxed mode, so this implementation cannot protect against
        arbitrary integer conversions. The 64bit implementation
        stands a better chance at that.
    */
    bytes += 1; // pointers may point to 1 past an array

    { // DEBUG
        auto again = std::find(alloc_list->begin(), alloc_list->end(), ptr);
        // should I do a more elaborate check here? To see if the ptr is contained 
        // within a preexisting allocation? 

        dbg_assert(again == alloc_list->end() && "register_alloc was called twice on the same pointer value"); 
    }

    // first fit
    bool done = false;
    for (size_t idx = 0; idx < free_list->size() && !done; idx++) {
        if ((done = (free_list->at(idx) && alloc_list->at(idx).size >= bytes))) {
            free_list->at(idx) = false;
            alloc_list->at(idx).allocation = ptr;
            if (alloc_list->at(idx).size > bytes) {
                alloc_entry new_alloc = { .allocation = nullptr, .size = alloc_list->at(idx).size - bytes };
                alloc_list->at(idx).size = bytes;
                alloc_list->insert(alloc_list->begin() + idx + 1, new_alloc);
                free_list->insert(free_list->begin() + idx + 1, true);
            }
        }
    }

    if (!done) { // no suitable free block available
        alloc_list->push_back({ .allocation = ptr, .size = bytes });
        free_list->push_back(false);
    }

    dbg_assert(free_list->size() == alloc_list->size());

    printf("`register_alloc`(%p, %lu) returns %p\n", ptr, bytes, ptr);
    // printf("Current metadata: \n");
    // print_metadata();

    return ptr;
}

void* register_alloc(void* ptr, size_t bytes) {
    printf("Register_alloc called with hook = %s \n", hook_status() ? "true" : "false");
    unhook_scope guard{};
    if (!ptr)  {
        printf("Attempt to register nullptr \n");
        return nullptr;
    }
    return force_register_alloc(ptr, bytes);
}

static void* force_unregister_alloc(void* ptr) {
    if (!alloc_list || !free_list)
        return ptr;
    unhook_scope guard{};
    { // iterator invalidation protection
        auto entry = std::find(alloc_list->begin(), alloc_list->end(), ptr);
        dbg_assert(entry != alloc_list->end());
        
        auto free = free_list->begin() + std::distance(alloc_list->begin(), entry);
        *free = true;

        // coalesc blocks
        auto leftmost = (free != free_list->begin() && *(free - 1)) ? entry - 1 : entry;
        auto rightmost = (((free != free_list->end() - 1) && *(free + 1)) ? entry + 1 : entry) + 1;
        for (auto it = leftmost + 1; it != rightmost; it++) {
            leftmost->size += it->size;
        }
        free_list->erase(
            free_list->begin() + std::distance(alloc_list->begin(), leftmost) + 1, 
            free_list->begin() + std::distance(alloc_list->begin(), rightmost)
        );
        alloc_list->erase(leftmost + 1, rightmost);
    }

    if (*free_list->rbegin()) {
        free_list->pop_back();
        alloc_list->pop_back();
    }

    dbg_assert(free_list->size() == alloc_list->size());
    dbg_assert(!*free_list->rbegin());

    printf("`unregister_alloc`(%p) returns %p\n", ptr, ptr);

    return ptr;
}

void* unregister_alloc(void* ptr) {
    unhook_scope guard{};
    if (ptr == nullptr) {
        printf("Attempt to unregister nullptr \n");
        return ptr;
    }
    return force_unregister_alloc(ptr);
}

uintptr_t unify(void* v_addr) {
    unhook_scope guard{};
    for (uintptr_t idx = 0, occ_idx = 0; idx < alloc_list->size(); idx++, occ_idx += alloc_list->at(idx).size) {
        printf("`unify(%p)`: currently investigating idx %lu and occ_idx %lu \n", v_addr, idx, occ_idx);
        if (v_addr >= alloc_list->at(idx).allocation && v_addr <= (static_cast<char*>(alloc_list->at(idx).allocation) + alloc_list->at(idx).size)) {
            uintptr_t ret = occ_idx + (static_cast<char*>(v_addr) - static_cast<char*>(alloc_list->at(idx).allocation));
            printf("`unify(%p)` returns %lu \n", v_addr, ret);
            return ret;
        }
    }

    printf("got here \n");

    printf("`unify(%p)` can't find %p \n", v_addr, v_addr);

    dbg_assert(false && "v_addr was not contained in any of the alloc_entries!");
    return 0;
}

void* deunify(uintptr_t u_addr) {
    unhook_scope guard{};
    for (uintptr_t idx = 0, occ_idx = 0; idx < alloc_list->size(); idx++, occ_idx += alloc_list->at(idx).size) {
        if (u_addr >= occ_idx && u_addr <= occ_idx + alloc_list->at(idx).size) {
            void* ret = static_cast<char*>(alloc_list->at(idx).allocation) + (u_addr - occ_idx);
            printf("`deunify(%lu)` returns %p \n", u_addr, ret);
            return ret;
        }
    }

    dbg_assert(false && "u_addr was not contained in any of the alloc_entries!");
    return 0;
}

size_t size_of_alloc(void* ptr) {
    unhook_scope guard{};
    auto entry = std::find(alloc_list->begin(), alloc_list->end(), ptr);
    dbg_assert(entry != alloc_list->end());
    return entry->size - 1;
}

// void print_metadata() {
//     std::cout << "----------------" << std::endl;
//     for (uintptr_t idx = 0, occ_idx = 0; idx < alloc_list->size(); idx++, occ_idx += alloc_list->at(idx).size) {
//         std::cout << "idx: " << idx << std::endl;
//         std::cout << "occ_idx: " << occ_idx << std::endl;
//         std::cout << "  allocation: " << alloc_list->at(idx).allocation << std::endl;
//         std::cout << "  size: " << alloc_list->at(idx).size << std::endl;
//         std::cout << "  free: " << free_list->at(idx) << std::endl;
//         std::cout << "----------------" << std::endl;
//     }
// }


// This is used for debugging and should not dynamically allocate memory 
// Actually with the guard now it can
void print_metadata() {
    unhook_scope guard{};
    printf("------START-----\n");
    printf("Size of alloc_list: %lu\n", alloc_list->size());
    printf("Size of free_list: %lu\n", free_list->size());
    printf("----------------\n");
    for (uintptr_t idx = 0, occ_idx = 0; idx < alloc_list->size(); idx++, occ_idx += alloc_list->at(idx).size) {
        char fmt[] = "idx: %lu \n" "occ_idx: %lu \n" "  allocation: %p \n" "  size: %lu \n" "  free: %s \n" "----------------\n";
        char buffer[1000] = {'\0'};
        sprintf(buffer, fmt, idx, occ_idx, alloc_list->at(idx).allocation, alloc_list->at(idx).size, free_list->at(idx) ? "true" : "false");
        printf("%s", buffer);
    }
    //printf("-----FINISH-----\n");
    //fflush(stdout);
}

}

run manage_vectors(
[](){
    unhook_scope guard{};
    alloc_list = new std::vector<alloc_entry>();
    free_list = new std::vector<bool>();
    force_register_alloc(nullptr, 0);
}, 
[](){
    unhook_scope guard{};
    delete alloc_list;
    delete free_list;
});


