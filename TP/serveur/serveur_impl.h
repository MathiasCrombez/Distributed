#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include <inttypes.h>

#include "hash.h"

typedef struct serveur {
    char * serveurname;
    uint64_t port;
    hash_s ** tabl;
    uint64_t firstKey;
    uint64_t size;
} serveur_s;

int put_h(serveur_s s, uint64_t cle, char * valeur, uint64_t taille);

//--------------------------------------------------------------------------------------
/**
 * La fonction get() recupère la valeur et sa taille associés à la clé et le serveur
 *
 * @param s est le serveur qui dispose de la clé (vérifié en amont)
 * @param cle est la clé qui identifie l'entier dans la table de hachage
 * @param valeur est un pointeur modifié dans la fonction pour 
 *        qu'il correspont à la valeur. C'est une sortie.
 *
 * @return size, la taille de valeur
 */
uint64_t get_h(serveur_s s, uint64_t cle, char * valeur);

int remove_h(serveur_s s, uint64_t cle, char * valeur, uint64_t taille);
