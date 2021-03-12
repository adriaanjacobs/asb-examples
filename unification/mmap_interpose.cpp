#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif 

#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>




extern "C" {


// void* __real_malloc(size_t bytes);

// void* __wrap_malloc(size_t bytes) {

//     printf("My malloc called \n");
//     return __real_malloc(bytes);
// }

void* __mmap (void *addr, size_t len, int prot, int flags, int fd, __off_t offset){
    static void* (*real_mmap)(void*, size_t, int, int, int, __off_t) = NULL;
    if (!real_mmap)
        real_mmap = (void* (*)(void*, size_t, int, int, int, __off_t))dlsym(RTLD_NEXT, "__mmap");
    

    void* ret = real_mmap(addr, len, prot, flags, fd, offset);
    int mmap_errno = errno;
    printf("My mmap called \n");

    errno = mmap_errno;
    return ret;
}

void* mmap (void *addr, size_t len, int prot, int flags, int fd, __off_t offset){
    return __mmap(addr, len, prot, flags, fd, offset);
}

void* mmap64 (void *addr, size_t len, int prot, int flags, int fd, __off_t offset) {
    return __mmap(addr, len, prot, flags, fd, offset);
}

}