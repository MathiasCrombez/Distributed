#ifndef SERVEUR_IMPL_H
#define SERVEUR_IMPL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>


#include "message.h"
#include "hash.h"


//=============================================================================
//			       MACROS
//=============================================================================

/*
 * NB de clients auxquels peut repondre un server en meme temps
 */
#define THREAD_MAX 5

/*
 * NB de Client accepté
 */
#define LENGTH_LISTEN_QUEUE 10

/*
 *permet de supprimer ou afficher les messages de debug
 */
#define DEBUG_SERVEUR_IMPL

/*
 * mettre son ip
 */
#define SERVEURNAME "127.0.0.1"


#define HASH_TABLE_SIZE (1<<4)

//==============================================================================
//				TYPES
//=============================================================================


/*
 *Identifie une connexion avec un client
 */
struct idClient {
	struct sockaddr_in identifiant;
	socket_t idSocket;
	char *name;
	pthread_t thread;
};


typedef struct tableauClient {
    struct idClient client;
    struct tableauClient * suiv;
} * tabClient_t;

typedef struct serveur {

	struct sockaddr_in serv_addr;
	socket_t idSocket;
	char name[20];

	uint64_t h;
	table_de_hachage_t tabl;
	
	tabClient_t tableauClient;
	idConnexion_t *suivServeur;
	idConnexion_t *precServeur;

} serveur_t;



//==============================================================================
//			VARIABLE GLOBAL
//==============================================================================


static serveur_t SERVEUR;


//==============================================================================
//			FONCTIONS
//==============================================================================

/** on creer un serveur.Il ne partage pas la DHT encor donc pas besoin de
 ** first_k, las_k et next en argument					**/
serveur_t* creerServeur(char *nomDuServeur, uint64_t port);
void *talk_to_client(void *sockClient);
void *talk_to_server(void *sockServer);



/*
 * renvoie les informations de mon serveur 
 */
idConnexion_t get_my_idConnexion();


/*
 * renvoie une pointeur vers la variable global SERVEUR
 */
serveur_t* get_my_server();


table_de_hachage_t* get_my_hashtab();

#endif

