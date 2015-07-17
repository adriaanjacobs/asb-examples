#ifndef HATASI_H
#define HATASI_H

#include <stddef.h>

size_t hash(void* ptr, size_t max);

struct KVPair {
        void* key;
        int val;
};

// mapping strings to ints
class Hatasi {
public:
        Hatasi();
        ~Hatasi();

        bool put(void*, int);
        int get(void*);
        bool remove(void*);
        
private:
        void newtable();
        void resize();
        size_t getTableIdx(void*);

        KVPair* table;
        size_t capacity;
        size_t size;
        size_t maxCollisions;
};

#endif
