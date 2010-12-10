#include"serveur_impl.h"
#include"message.h"

#define DEBUG_MESSAGE_SERVEUR
//#undef  DEBUG_MESSAGE_SERVEUR

#ifdef DEBUG_MESSAGE_SERVEUR
#   define PRINT_DEBUG(chaine, args...)   printf(chaine,##args)
#else
#   define PRINT_DEBUG(chaine, args...)
#endif







int messageConnect2Server(char *ip, uint64_t port)
{
/*	struct in_addr ip_suiv;*/
/*	uint32_t next_server_port;*/
/*	socket_t idSocket;*/
	socket_t new_socket;
	socket_t sockServer;
	char reponse=0;
	
	sockServer = connect2server(ip, port);
	if (sockServer == 0) {
		printf("echec de la connexion\n");
		return 0;
	}
	PRINT_DEBUG("*****connexion au serveur réussie*****\n");
	
	envoyerOrigine(FROM_SERVEUR,sockServer);
	envoyerTypeMessage(CONNECT,sockServer);
	printf("envoie du premier oct\n");
	envoyerOctet(0,sockServer);
	recevoirIdent(&(SERVEUR.precServeur->identifiant), sockServer);
	recevoirChaine(&(SERVEUR.precServeur->name), sockServer);
	
	recevoirIdent(&(SERVEUR.suivServeur->identifiant), sockServer);
	recevoirChaine(&(SERVEUR.suivServeur->name), sockServer);
	
	envoyerIdent(SERVEUR.serv_addr,sockServer);
	envoyerChaine(SERVEUR.name,sockServer);
	
	shutdown(sockServer,SHUT_RDWR);
	
	new_socket=connect2server(inet_ntoa(SERVEUR.suivServeur->identifiant.sin_addr),ntohs(SERVEUR.suivServeur->identifiant.sin_port));
	envoyerOrigine(FROM_SERVEUR,new_socket);
	envoyerTypeMessage(CONNECT,new_socket);
	printf("envoie du second oct\n");
	envoyerOctet(1,new_socket);
	envoyerIdent(SERVEUR.serv_addr, new_socket);	
	envoyerChaine(SERVEUR.name, new_socket);

		
	shutdown(new_socket,SHUT_RDWR);
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
int message_disconnect_from_server(socket_t sockClient,origine_t origine){

	envoyerOrigine(origine,sockClient);
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

