#ifndef CLIENT_IMPL_H
#define CLIENT_IMPL_H



#include "commun.h"


//=============================================================================
//			       MACROS
//=============================================================================

/*
 *permet de supprimer ou afficher les messages de debug
 */
#define DEBUG_CLIENT_IMPL
#undef DEBUG_CLIENT_IMPL



//==============================================================================
//				TYPES
//=============================================================================


typedef struct client {
	const char *monNom;

	struct sockaddr_in serv_addr;
	socket_t idSocket;
	
} client_t;



//==============================================================================
//			VARIABLE GLOBAL
//==============================================================================

static client_t CLIENT;



//==============================================================================
//			FONCTIONS
//==============================================================================


client_t* creerClient(const char *nom);
socket_t connect2server(char* to_serveur,uint64_t port);


//uint32_t put(uint32_t cle, uint32_t valeur);/** Envoie un couple clé valeur **/
//uint32_t get(uint32_t cle);/** Reçoit la valeur associé à la clé **/
//uint32_t removeKey(uint32_t cle);/** Supprime une entrée de la DHT **/
//uint32_t disconnect2server();


//uint32_t quit(char * nomDuServeur);
#endif
