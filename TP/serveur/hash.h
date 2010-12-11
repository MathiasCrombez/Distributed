#ifndef HASH_H
#define HASH_H


#include "commun.h"


/* 
 * taille maximal d'une table de hachage 
 */
#define MAX_TAILLE_HASH_TABLE 1<<16

typedef struct Table_De_Hachage {
    uint64_t taille;
    /**
     * L'utilisation d'un pointeur permet d'avoir une table de hachage
     * de taille dynamique.C'est utile car dans une dht , chaque processus
     * detenant une partie de la table de hachage devra éventuellement partager
     * sa table avec un autre processus serveur
     */
    liste_t *table_de_hachage;
} table_de_hachage_t;



/*
 * creer et liberer une table de hachage 
 */
table_de_hachage_t creerHashTable(uint64_t taille);
void libererHashTable(table_de_hachage_t hashTab);


/*
 * les trois opérations de base de la TH
 */
donnee_t getHashTable(char *clef,table_de_hachage_t hashTab);
void putHashTable(donnee_t data,table_de_hachage_t hashTab);
valeur_t removeHashTable(char *cle,table_de_hachage_t hashTab);

/*
 * fonction de hachage 
 */
inline uint64_t hash(cle_t K);

/*
 * partage une table de hachage 
 */
void segmenterHashTable(uint64_t indice,
                        table_de_hachage_t * hash_tab1_ptr,
                        table_de_hachage_t * hash_tab2_ptr);
                        
                        




#endif

