#ifndef HASH_H
#define HASH_H

#include <inttypes.h>
#include "commun.h"



/** la table de hachage a une taille dynamique  :             **/
/**         	elle pourra  être coupé , tranché etc         **/
/** on utilise donc un pointeur et nom un tableau en dur pour **/
/** representer la table de hachage							  **/


#define MAX_TAILLE_HASH_TABLE 1<<16 
static uint64_t TAILLE_HASH_TABLE;
static liste_t* tableDeHachage;

liste_t* creerHashTable(uint64_t taille);
donnee_t getHashTable(char* clef);
void putHashTable(donnee_t data);
donnee_t removeHashTable(donnee_t data);


static inline uint64_t hash(char* str) {
	 uint64_t hash = 5381;
    
    assert(str!=NULL);  
#ifdef DEBUG_MESSAGE
	printf("la cle est: %s de longueur: %d \n",str,strlen(str));
#endif
	
    while(*str!='\0') {
        int c = *str;
        hash = ((hash << 5) + hash) + c;
        str ++;
    }
    return (hash % TAILLE_HASH_TABLE);
}
#endif
