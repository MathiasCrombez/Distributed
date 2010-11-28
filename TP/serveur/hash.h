#ifndef HASH_H
#define HASH_H

#include <inttypes.h>
#include "commun.h"


static uint64_t TAILLE_HASH_TABLE;
static liste_t* tableDeHachage;

/** la table de hachage a une taille dynamique  :             **/
/**         	elle pourra  être coupé , tranché etc         **/
/** on utilise donc un pointeur et nom un tableau en dur pour **/
/** representer la table de hachage							  **/


#define MAX_TAILLE_HASH_TABLE 1<<16 



liste_t* creerHashTable(uint64_t taille);

/** les trois opérations de base de la TH**/
donnee_t getHashTable(char* clef);
void putHashTable(donnee_t data);
char* removeHashTable(char* cle);


inline uint64_t hash(char* str);
#endif
