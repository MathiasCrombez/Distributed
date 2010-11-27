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



#define SERVEURNAME "127.0.0.1"
#define PORT 4242




typedef uint32_t socket_t;


struct client {
	const char *monNom;
	uint32_t port;

	struct sockaddr_in serv_addr;
	socket_t idSocket;

	uint32_t cle;
	uint32_t cleDuSuccesseur;

};


typedef struct client client_t;








union query {

	struct suite_query {} quit;
	
	struct state_query {} state;
	
	struct put_query {
	
	
	
	} put;
	
	struct get_query {
	
	
	} get;
	
	struct remove_query {
	
		
	} remove;
};

enum type_message { QUIT, STATE, PUT, GET, REMOVE };

struct message {

	enum type_message type;
	union  query requete;
	
};
typedef struct message message_t;

client_t creerClient(const char *nom, uint32_t k);
uint32_t connect2server(client_t client);
uint32_t envoyerMessage(message_t message,socket_t serveur);


uint32_t put(uint32_t cle, uint32_t valeur);/** Envoie un couple clé valeur **/
uint32_t get(uint32_t cle);/** Reçoit la valeur associé à la clé **/
uint32_t removeKey(uint32_t cle);/** Supprime une entrée de la DHT **/
uint32_t disconnect2server();
#endif
