#include <unify.h>
#include <iostream>
#include <vector>
#include <algorithm>

static std::vector<void*> occurrences;

extern "C" uintptr_t unify(void* v_addr) {
    if (auto it = std::find(occurrences.begin(), occurrences.end(), v_addr); it != occurrences.end())
        return it - occurrences.begin();
    occurrences.push_back(v_addr);
    return occurrences.size() - 1;
}

extern "C" void* deunify(uintptr_t u_addr) {
    return occurrences.at(u_addr);
}