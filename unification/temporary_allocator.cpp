#include "temporary_allocator.h"
#include "mmap_vector.h"
#include <sys/mman.h>


void* temporary_allocator::temp_alloc(size_t bytes) {
    void* ret = mmap(nullptr, bytes, PROT_READ|PROT_WRITE, MAP_ANONYMOUS, -1, 0);
    metadata.add({
        .base = ret,
        .bytes = bytes
    });
    return ret;
}

void temporary_allocator::temp_free(void* ptr) {
    int idx = metadata.find_if([ptr](const alloc_data& data) -> bool {
        return ptr >= data.base && ptr < static_cast<char*>(data.base) + data.bytes;
    });
    assert(idx > 0 && "Attempted to temp_free a pointer that was not temp_allocced");
    munmap(metadata.at(idx).base, metadata.at(idx).bytes);
    metadata.remove_at(idx);
}

void* temporary_allocator::temp_memalign(size_t alignment, size_t size) {
    size_t bytes = alignment + size;
    char* ret = static_cast<char*>(temp_alloc(bytes));
    size_t offset = alignment - (((size_t) ret) % alignment);
    assert(offset < bytes);
    ret += offset;
    assert(((size_t)ret) % alignment == 0);
    return ret;
}



