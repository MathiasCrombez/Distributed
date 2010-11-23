#include "hash.h"

uint64_t hash(unsigned char *str) {
    uint64_t hash = 5381;
    while(*str!='\0') {
        int c = *str;
        // hash = hash * 33 + c
        hash = ((hash << 5) + hash) + c;
        str ++;
    }
    return hash;
}
