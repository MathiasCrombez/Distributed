#ifndef MESSAGE_H
#define MESSAGE_H


#include "message_commun.h"

socket_t message_connect(idConnexion_t * server);

donnee_t  message_get(cle_t K,socket_t from);
void  message_put(cle_t K,socket_t from);
valeur_t message_remove(cle_t K,socket_t from);
void message_disconnect(socket_t from);
#endif
