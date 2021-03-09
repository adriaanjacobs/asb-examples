#include <unify.h>

#include <vector>
#include <cstdint>
#include <algorithm>
#include <assert.h>
#include <iostream>

static_assert(sizeof(char) == 1);

struct alloc_entry {
    void* allocation;
    size_t size; // 1 byte bigger than the allocated size

    bool operator == (void* other) const {
        return allocation == other;
    }
};

// again, SOA thing
std::vector<alloc_entry> alloc_list{};
std::vector<bool> free_list{};

extern "C" {

void* register_alloc(void* ptr, size_t bytes) {
    printf("Register_alloc called for %p (%lu bytes)\n", ptr, bytes);
    bytes += 1; // pointers may point to 1 past an array

    { // DEBUG
        auto again = std::find(alloc_list.begin(), alloc_list.end(), ptr);
        assert(again == alloc_list.end());
    }

    // first fit
    bool done = false;
    for (size_t idx = 0; idx < free_list.size() && !done; idx++) {
        if ((done = (free_list[idx] && alloc_list[idx].size >= bytes))) {
            free_list[idx] = false;
            alloc_list[idx].allocation = ptr;
            if (alloc_list[idx].size > bytes) {
                alloc_entry new_alloc = { .allocation = nullptr, .size = alloc_list[idx].size - bytes };
                alloc_list[idx].size = bytes;
                alloc_list.insert(alloc_list.begin() + idx + 1, new_alloc);
                free_list.insert(free_list.begin() + idx + 1, true);
            }
        }
    }

    if (!done) { // no suitable free block available
        alloc_list.push_back({ .allocation = ptr, .size = bytes });
        free_list.push_back(false);
    }

    assert(free_list.size() == alloc_list.size());

    return ptr;
}

void* unregister_alloc(void* ptr) {
    printf("Unregister_alloc called for %p\n", ptr);
    { // iterator invalidation protection
        auto entry = std::find(alloc_list.begin(), alloc_list.end(), ptr);
        assert(entry != alloc_list.end());
        auto free = free_list.begin() + std::distance(alloc_list.begin(), entry);
        *free = true;

        // coalesc blocks
        auto leftmost = (free != free_list.begin() && *(free - 1)) ? entry - 1 : entry;
        auto rightmost = (((free != free_list.end() - 1) && *(free + 1)) ? entry + 1 : entry) + 1;
        for (auto it = leftmost + 1; it != rightmost; it++) {
            leftmost->size += it->size;
        }
        free_list.erase(
            free_list.begin() + std::distance(alloc_list.begin(), leftmost) + 1, 
            free_list.begin() + std::distance(alloc_list.begin(), rightmost)
        );
        alloc_list.erase(leftmost + 1, rightmost);
    }

    if (*free_list.rbegin()) {
        free_list.pop_back();
        alloc_list.pop_back();
    }

    assert(free_list.size() == alloc_list.size());
    printf("Return from unregister_alloc with list sizes = %lu\n", free_list.size());
    assert(!*free_list.rbegin());
    return ptr;
}

uintptr_t unify(void* v_addr) {
    for (uintptr_t idx = 0, occ_idx = 0; idx < alloc_list.size(); idx++, occ_idx += alloc_list[idx].size) {
        if (v_addr >= alloc_list[idx].allocation && v_addr <= (static_cast<char*>(alloc_list[idx].allocation) + alloc_list[idx].size))
            return occ_idx + (static_cast<char*>(v_addr) - static_cast<char*>(alloc_list[idx].allocation));
    }

    assert(false && "v_addr was not contained in any of the alloc_entries!");
    return 0;
}

void* deunify(uintptr_t u_addr) {
    for (uintptr_t idx = 0, occ_idx = 0; idx < alloc_list.size(); idx++, occ_idx += alloc_list[idx].size) {
         if (u_addr >= occ_idx && u_addr <= occ_idx + alloc_list[idx].size)
            return static_cast<char*>(alloc_list[idx].allocation) + (u_addr - occ_idx);
    }

    assert(false && "u_addr was not contained in any of the alloc_entries!");
    return 0;
}

size_t size_of_alloc(void* ptr) {
    auto entry = std::find(alloc_list.begin(), alloc_list.end(), ptr);
    assert(entry != alloc_list.end());
    return entry->size - 1;
}

// void print_metadata() {
//     std::cout << "----------------" << std::endl;
//     for (uintptr_t idx = 0, occ_idx = 0; idx < alloc_list.size(); idx++, occ_idx += alloc_list[idx].size) {
//         std::cout << "idx: " << idx << std::endl;
//         std::cout << "occ_idx: " << occ_idx << std::endl;
//         std::cout << "  allocation: " << alloc_list.at(idx).allocation << std::endl;
//         std::cout << "  size: " << alloc_list.at(idx).size << std::endl;
//         std::cout << "  free: " << free_list.at(idx) << std::endl;
//         std::cout << "----------------" << std::endl;
//     }
// }


// This is used for debugging and should not dynamically allocate memory (to simplify)
void print_metadata() {
    printf("------START-----\n");
    printf("Size of alloc_list: %lu\n", alloc_list.size());
    printf("Size of free_list: %lu\n", free_list.size());
    printf("----------------\n");
    for (uintptr_t idx = 0, occ_idx = 0; idx < alloc_list.size(); idx++, occ_idx += alloc_list[idx].size) {
        char fmt[] = "idx: %lu \n" "occ_idx: %lu \n" "  allocation: %p \n" "  size: %lu \n" "  free: %s \n" "----------------\n";
        char buffer[1000] = {'\0'};
        sprintf(buffer, fmt, idx, occ_idx, alloc_list.at(idx).allocation, alloc_list.at(idx).size, free_list.at(idx) ? "true" : "false");
        printf("%s", buffer);
    }
    printf("-----FINISH-----\n");
}

}


