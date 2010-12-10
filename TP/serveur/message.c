#include"serveur_impl.h"
#include"message.h"

#define DEBUG_MESSAGE_SERVEUR
//#undef  DEBUG_MESSAGE_SERVEUR

#ifdef DEBUG_MESSAGE_SERVEUR
#   define PRINT_DEBUG(chaine, args...)   printf(chaine,##args)
#else
#   define PRINT_DEBUG(chaine, args...)
#endif




int messageConnect2Server(char *to_serveur, uint64_t port)
{
	struct in_addr ip_suiv;
	uint32_t next_server_port;
	socket_t idSocket;
	socket_t new_socket;
	
	idSocket = connect2server(to_serveur, port);
	if (idSocket == 0) {
		printf("echec de la connexion\n");
		return 0;
	}
	SERVEUR.precServeur->idSocket = idSocket;
	
	
	PRINT_DEBUG("connexion au serveur réussie\n");
	

	/* envoi du type de message au serveur */
	envoyerTypeMessage(CONNECT, idSocket);

	// reception des champs de precServeur
	recevoirIdent(&(SERVEUR.precServeur->identifiant), idSocket);
	recevoirChaine(&(SERVEUR.precServeur->name), idSocket);
	
	// reception des champs de suivServeur
	recevoirIdent(&SERVEUR.suivServeur->identifiant, idSocket);
	recevoirChaine(&(SERVEUR.suivServeur->name), idSocket);
	
	//reception de firstKey et nextKey
	recevoirHash(&(SERVEUR.precKey), idSocket);
	recevoirHash(&(SERVEUR.nextKey), idSocket);
	
	//envoi de mes identifiants
	envoyerIdent(SERVEUR.serv_addr, idSocket);
	envoyerChaine(SERVEUR.name, idSocket);
	
	envoyerHash(SERVEUR.firstKey, idSocket);
	
	
	
	//connexion au nouveau serveur
	next_server_port = SERVEUR.suivServeur->identifiant.sin_port;
	ip_suiv = SERVEUR.suivServeur->identifiant.sin_addr;
	
	printf("Connexion au nouveau serveur");
	new_socket=connect2server(inet_ntoa(ip_suiv), ntohs(next_server_port));
	SERVEUR.suivServeur->idSocket = idSocket;
	


	//#### debug ####//
	printf("****serveur precedent est:****\n");
	afficherIdentConnexion(SERVEUR.precServeur);
	printf("\n");
	printf("****serveur suivant est:****\n");
	afficherIdentConnexion(SERVEUR.suivServeur);
	printf("\n");
	//#### debug ####//
		
	return 1;
}


//coupe la socket passé en arg et indique au serveur de couper la communication
int message_disconnect_from_server(socket_t sockClient){

	envoyerTypeMessage(DISCONNECT,sockClient);
	if(shutdown(sockClient,SHUT_RDWR)==-1){
		perror("shutdown()");
		return 0;
	}
	return 1;
}


int messageParticiperDHT()
{

}

