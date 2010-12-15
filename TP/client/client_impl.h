#ifndef CLIENT_IMPL_H
#define CLIENT_IMPL_H



#include "commun.h"


//=============================================================================
//			       MACROS
//=============================================================================

/*
 *permet de supprimer ou afficher les messages de debug
 */
//#define DEBUG_CLIENT_IMPL
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

//static client_t CLIENT;



//==============================================================================
//			FONCTIONS
//==============================================================================


client_t creerClient(const char *nom);
idConnexion_t * preConnect(char *to_serveur, uint64_t port);
socket_t connect2server(idConnexion_t * server, client_t client);

#endif
