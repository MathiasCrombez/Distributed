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
//#undef DEBUG_CLIENT_IMPL



//==============================================================================
//				TYPES
//=============================================================================


typedef struct client {
	char name[20];

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

socket_t ___connect2server___(struct sockaddr_in server_info);

#endif
