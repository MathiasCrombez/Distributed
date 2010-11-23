#include <inttypes.h>

typedef struct hash {
    uint64_t key;
    uint64_t size;
    char * value;

    //    struct hash * suiv;
    /* il y aura des casts mais on ne peut pas manipuler des pointeurs ainsi
     * car on ne connait pas la taille de l'adresse
     */
    uint64_t suiv;
} hash_s;

uint64_t hash(unsigned char *str);
