#ifndef MESSAGE_H
#define MESSAGE_H
/**
 * Ces fonctions sont les messages que le client envoit
 *
 *
 */

#include "message_commun.h"

socket_t message_connect(char* ip, uint32_t port);

void  message_put(donnee_t D,socket_t from);
donnee_t  message_get(cle_t K, socket_t from);
valeur_t message_remove(cle_t K,socket_t from);
void message_disconnect(socket_t from);

void message_quit(socket_t from);
idConnexion_t message_status(socket_t from);
#endif
