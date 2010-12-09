#ifndef SERVEUR_IMPL_H
#define SERVEUR_IMPL_H


#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include "message.h"
#include <netdb.h>


#include "hash.h"
#include "message.h"
#define THREAD_MAX 5
#define LENGTH_LISTEN_QUEUE 10
#define MESSAGE_SIZE 256





struct idConnexion {

	struct sockaddr_in identifiant;
	socket_t idSocket;
	char* name;
};

typedef struct serveur {

    struct sockaddr_in serv_addr;
    socket_t idSocket;
    char name[20];
    
    table_de_hachage_t tabl;
    uint64_t firstKey;
    uint64_t nextKey;

    struct idConnexion tableauClient[LENGTH_LISTEN_QUEUE];
    struct idConnexion nextServeur;
    
} serveur_t;


//## variable global ##/
serveur_t SERVEUR;


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

uint32_t connect2server( char* to_serveur,uint64_t port);
int messageConnect2Server(char* ip, uint64_t port);


static void afficherIdentConnexion(struct idConnexion ident){
	
	printf("identifiant connexion:\n");
	printf("\tNom       : %s\n",ident.name);
	printf("\tAdresse Ip: %s\n", inet_ntoa(ident.identifiant.sin_addr)); 
	printf("\tPort conne: %d\n", ntohs(ident.identifiant.sin_port));

}
#endif
