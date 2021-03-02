#include "hatasi.h"
#include <stdio.h>
#include <stdlib.h>

size_t hash(void* ptr, size_t max) {
        return ((size_t) ptr) % max;
}

Hatasi::Hatasi() {
        capacity = 20;
        maxCollisions = 10;
        size = 0;
        newtable();
}

Hatasi::~Hatasi() {
        delete[] table;
}

void Hatasi::newtable() {
        size_t tabsize = capacity * maxCollisions;

        printf("newtable size %ld\n", tabsize);
        table = new KVPair[tabsize];
        
        for (size_t i = 0; i < tabsize; ++i) {
                table[i].key = NULL; // set the key to NULL
        }
}

void Hatasi::resize() {
        printf("Resize called\n");
        size_t oldcap = capacity;
        size_t oldMaxCollisions = maxCollisions;
        KVPair* oldtab = table;
        
        capacity *= 2;
        maxCollisions++;
        newtable();
        
        for (size_t i = 0; i < oldcap * oldMaxCollisions; ++i) {
                void* k = oldtab[i].key;
                if (k != NULL) {
                        put(k, oldtab[i].val);
                }
        }
        
        delete[] oldtab;
}

size_t Hatasi::getTableIdx(void* key) {
        size_t h = hash(key, capacity);
        return h * maxCollisions;
}

bool Hatasi::put(void* key, int value) {
        if (key == NULL) {
                return false;
        }

        size_t baseIdx = getTableIdx(key);
        
        size_t i;
        for (i = 0; i < maxCollisions; ++i) {
                void* elkey = table[baseIdx + i].key;
                if (elkey == NULL) { // this element is still unused
                        break; // insert this element here
                } else if (elkey == key) { // there is already a value with this key
                        return false;
                }
        }
        
        // if there is still space at this hash idx
        if (i < maxCollisions) {
                table[baseIdx + i].key = key;
                table[baseIdx + i].val = value;
                size++;
                return true;
        } else {
                resize();
                return put(key, value);
        }
}

int Hatasi::get(void* key) {
        if (key == NULL) {
                return 0;
        }

        size_t baseIdx = getTableIdx(key);
        
        size_t i;
        for (i = 0; i < maxCollisions; ++i) {
                void* elkey = table[baseIdx + i].key;
                if (elkey == NULL) { // this element is still unused
                        break;
                } else if (elkey == key) { // there is a value with this key
                        return table[baseIdx + i].val;
                }
        }
        
        return -1;
}

bool Hatasi::remove(void* key) {
        if (key == NULL) {
                return false;
        }

        size_t baseIdx = getTableIdx(key);
        
        size_t i;
        for (i = 0; i < maxCollisions; ++i) {
                void* elkey = table[baseIdx + i].key;
                if (elkey == NULL) { // this element is still unused
                        return false;
                } else if (elkey == key) { // there is a value with this key
                        // remove the value; by shifting all the next values here
                        for (size_t j = i+1; j < maxCollisions; ++j, ++i) {
                                table[baseIdx + i].key = table[baseIdx + j].key;
                                table[baseIdx + i].val = table[baseIdx + j].val;
                        }
                        table[baseIdx + i].key = NULL;
                        size--;
                        
                        return true;
                }
        }
        
        return false;
}
