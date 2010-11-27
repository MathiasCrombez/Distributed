#ifndef MESSAGE_H
#define MESSAGE_H

#include "commun.h"

#define MAXCAR 20

union query {

	struct suite_query {
		char nomDuServeur [MAXCAR];
		uint32_t sEstDeconnecte;
		
	} quit;
	
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
	union query requete;
	
};

typedef struct message message_t;
uint32_t envoyerMessage(message_t message,socket_t destinataire);
uint32_t recevoirMessage(message_t message, socket_t expediteur);
#endif
