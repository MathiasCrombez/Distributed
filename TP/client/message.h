#ifndef MESSAGE_H
#define MESSAGE_H


#include "message_commun.h"
#include "client_impl.h"
socket_t message_connect(idConnexion_t * server, client_t client);
void  message_put(donnee_t D,socket_t from, client_t client);
donnee_t  message_get(cle_t K, socket_t from, client_t client);
valeur_t message_remove(cle_t K,socket_t from, client_t client);
void message_disconnect(socket_t from, client_t client);
#endif
