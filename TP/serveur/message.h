#ifndef MESSAGE_H
#define MESSAGE_H

#include"message_commun.h"
#include"liste_donnee.h"

//==============================================================================
//			       MACROS
//==============================================================================


//#define DEBUG_MESSAGE_SERVEUR
//#undef  DEBUG_MESSAGE_SERVEUR

#ifdef DEBUG_MESSAGE_SERVEUR
#   define PRINT_DEBUG(chaine, args...)   printf(chaine,##args)
#else
#   define PRINT_DEBUG(chaine, args...)
#endif




/*
 * connect au serveur le moins chargé
 */
int message_connect_2_server(char *ip, uint32_t port);

/*
 * se deconnecte du serveur
 */
int message_disconnect_from_server(socket_t sockClient);




#endif
