#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "message.h"
#include "commun.h"

#define SERVEURNAME "127.0.0.1"



struct client {
	const char *monNom;
	uint32_t port;

	struct sockaddr_in serv_addr;
	socket_t idSocket;

	uint32_t cle;
	uint32_t cleDuSuccesseur;

};


typedef struct client client_t;








client_t creerClient(const char *nom, uint32_t k);
uint32_t connect2server(client_t client);


uint32_t put(uint32_t cle, uint32_t valeur);/** Envoie un couple clé valeur **/
uint32_t get(uint32_t cle);/** Reçoit la valeur associé à la clé **/
uint32_t removeKey(uint32_t cle);/** Supprime une entrée de la DHT **/
uint32_t disconnect2server();


uint32_t quit(char * nomDuServeur);
#endif
