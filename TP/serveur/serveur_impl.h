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
#define SERVEURNAME "194.254.210.81"

//==============================================================================
//				TYPES
//=============================================================================

/*
 * identifie une connexion.
 * contient l'ip , le port , le nom du client et un socket pour
 * communiquer avec lui
 */ 
struct idConnexion {

	struct sockaddr_in identifiant;
	socket_t idSocket;
	char *name;
	pthread_t thread;
};



typedef struct serveur {

	struct sockaddr_in serv_addr;
	socket_t idSocket;
	char name[20];

	table_de_hachage_t tabl;
	uint64_t firstKey;
	uint64_t nextKey;
	uint64_t precKey;

	struct idConnexion tableauClient[LENGTH_LISTEN_QUEUE];
	struct idConnexion *suivServeur;
	struct idConnexion *precServeur;

} serveur_t;



//==============================================================================
//			VARIABLE GLOBAL
//==============================================================================

serveur_t SERVEUR;



//==============================================================================
//			FONCTIONS
//==============================================================================

/** on creer un serveur.Il ne partage pas la DHT encor donc pas besoin de
 ** first_k, las_k et next en argument								**/
serveur_t* creerServeur(char *nomDuServeur, uint64_t port);
void *talk_to_client(void *sockClient);
void *talk_to_server(void *sockServer);
socket_t connect2server(char *to_serveur, uint64_t port);
int message_connect_2_server(char *ip, uint64_t port);


/*
 * fonction de debug
 */
static void afficherIdentConnexion(struct idConnexion *ident)
{

	printf("identifiant connexion:\n");
	printf("\tNom       : %s\n", ident->name);
	printf("\tAdresse Ip: %s\n", inet_ntoa(ident->identifiant.sin_addr));
	printf("\tPort conne: %d\n", ntohs(ident->identifiant.sin_port));
}


#endif

