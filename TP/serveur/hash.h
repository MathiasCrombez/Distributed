#include <inttypes.h>

typedef struct hash {
    //    uint64_t key;
    uint64_t size;
    char ** value;
} hash_s;

uint64_t hash(unsigned char *str);
