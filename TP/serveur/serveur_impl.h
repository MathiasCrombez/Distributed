#ifndef SERVEUR_IMPL_H
#define SERVEUR_IMPL_H


#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>


#include "hash.h"
#include "message.h"
#define THREAD_MAX 5
#define LENGTH_LISTEN_QUEUE 10
#define MESSAGE_SIZE 256



struct idServeur {
    char serveurname[MAXCAR];
    uint64_t port;
} ;


struct idClient {

    struct sockaddr_in client_addr;
    socket_t idSocket;
};

typedef struct serveur {
    
    struct idServeur s;
    struct sockaddr_in serv_addr;
    socket_t idSocket;
	
    struct idClient tableauClient[LENGTH_LISTEN_QUEUE];
	
    table_de_hachage_t tabl;
    uint64_t firstKey;
    uint64_t size;
    
    struct idServeur* next_serv;
} serveur_t;


/** on creer un serveur.Il ne partage pas la DHT encor donc pas besoin de
 ** first_k, las_k et next en argument								**/
serveur_t creerServeur(char* nomDuServeur, uint64_t port);
							
void talk_to_client(socket_t  socket);

//int put_h( uint64_t cle, char * valeur, uint64_t taille);

///**********************************************************/
///**
// * La fonction get() recupère la valeur et sa taille associés à la clé et le serveur
// *
// * @param s est le serveur qui dispose de la clé (vérifié en amont)
// * @param cle est la clé qui identifie l'entier dans la table de hachage
// * @param valeur est un pointeur modifié dans la fonction pour 
// *        qu'il correspont à la valeur. C'est une sortie.
// *
// * @return size, la taille de valeur
// */
//uint64_t get_h(cle_t cle, valeur_t valeur);

//int remove_h(cle_t cle, valeur, uint64_t taille);


#endif
