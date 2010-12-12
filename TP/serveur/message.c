#include"serveur_impl.h"
/**
 *Implémente les messages que s'envoient les serveurs entres eux seulement
 **/




/*
 * Connect à un server.Renvoi le socket qui permet de communiquer avec le client
 */
inline socket_t connect2server(struct sockaddr_in server_info)
{
	socket_t idSocket;
	
	idSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (idSocket < 0) {
		perror("socket()");
		exit(-1);
	}

	if (connect(idSocket, (struct sockaddr *)&server_info,sizeof(struct sockaddr_in)) == -1) {
		perror("connect()");
		return 0;
	}

	return idSocket;
}

/*
* renvoi les info de connexion du prochain serveur 
*/
inline idConnexion_t* message_whois_next_server(struct sockaddr_in serv_addr){
 
 	idConnexion_t* next_server_info;
 	socket_t sock_next_server;
 	
 	sock_next_server=connect2server(serv_addr);
	if (sock_next_server == 0) {
		perror("echec de la connexion\n");
		exit(-1);
	}
		
 	/* communication avec le serveur */
 	envoyerOrigine(FROM_SERVEUR,sock_next_server);
 	envoyerTypeMessage(WHOIS_NEXT_SERVER,sock_next_server);
 	
 	recevoirIdent(&next_server_info, sock_next_server);
 	return next_server_info;
 }
 

inline idConnexion_t* message_ident(struct sockaddr_in serv_addr){
	
	idConnexion_t* server_info;
	socket_t sock_next_server;
 	
 	sock_next_server=connect2server(serv_addr);
	if (sock_next_server == 0) {
		perror("echec de la connexion\n");
		exit(-1);
	}
	envoyerOrigine(FROM_SERVEUR,sock_next_server);
 	envoyerTypeMessage(IDENT,sock_next_server);
 	
 	recevoirIdent(&server_info, sock_next_server);
 	return server_info;
}


/*
 * Connection au server d'adresses IP ip sur le port port.
 * Equilibrage de charge.Apres un parcour rapide de la dht, on décide de la
 * valeur du h
 */
int message_connect_2_server(char* ip,uint32_t port){

	uint64_t taille_max;
	socket_t sockClient;
	struct hostent *hostinfo ;
	struct sockaddr_in  serv_addr;
	idConnexion_t* iterator_server_info;
	idConnexion_t* server_most_charged;
	
	
	hostinfo = gethostbyname(ip);
	if (hostinfo == NULL) {
		printf("erreur gethostbyname():le serveur %s est inconnu\n",ip);
		return 0;
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(port);
	serv_addr.sin_addr   = *(struct in_addr *)hostinfo->h_addr;
	
	
	//itinialisation
	taille_max = 0;
	server_most_charged = NULL;
	
	while(1){	

		iterator_server_info= message_ident(serv_addr);
		assert(iterator_server_info!=NULL);
		
		if(iterator_server_info->taille_hashtab > taille_max){
			taille_max = iterator_server_info->taille_hashtab ;
			server_most_charged =  iterator_server_info;
		}
		iterator_server_info=message_whois_next_server(serv_addr);
		serv_addr = iterator_server_info->identifiant;
		shutdown(sockClient,SHUT_RDWR);
	
		if( serv_addr.sin_port==htons(port) ) {
			
			return message_connect_to(server_most_charged->identifiant);
		}
	}
}




inline int message_connect_to(struct sockaddr_in server_info)
{
	socket_t new_socket;
	socket_t sockServer;
	char reponse=0;
	idConnexion_t id_connexion;
	/* 
	 * Connexion au serveur dont les identifiants sont dans server_info
	 * recuperation des identifiants du serveur suivant dans le cercle
	 */
	sockServer = connect2server(server_info);
	if (sockServer == 0) {
		printf("echec de la connexion\n");
		exit(-1);
	}
	
	envoyerOrigine(FROM_SERVEUR,sockServer);
	envoyerTypeMessage(CONNECT,sockServer);
	
	envoyerOctet(0,sockServer);
	recevoirIdent(&SERVEUR.precServeur, sockServer);
	recevoirIdent(&SERVEUR.suivServeur, sockServer);
	id_connexion= get_my_idConnexion();
	envoyerIdent(&id_connexion,sockServer);
	/*
	 *on libere la connexion 
	 */
	if (shutdown(sockServer,SHUT_RDWR)) {
        	printf("echec de la premiere deconnection du serveur\n");
         	exit(-1);
        }
	
	
	/* 
	 * Connexion au serveur suivant dans le cercle
	 * envoi de mes identifiants au serveur suivant
	 */
	new_socket=connect2server(SERVEUR.suivServeur->identifiant);
	if (sockServer == 0) {
		printf("echec de la connexion\n");
		exit(-1);
	}
	
	envoyerOrigine(FROM_SERVEUR,new_socket);
	envoyerTypeMessage(CONNECT,new_socket);
	
	envoyerOctet(1,new_socket);
	id_connexion= get_my_idConnexion();
	envoyerIdent(&id_connexion, new_socket);	
	/*
	 *on libere la connexion 
	 */	
	if (shutdown(new_socket,SHUT_RDWR)) {
        	printf("echec de la deuxieme deconnection du serveur\n");
            	exit(-1);
        }

#ifdef DEBUG_MESSAGE_SERVEUR
	printf("****serveur precedent est:****\n");
	afficherIdentConnexion(SERVEUR.precServeur);
	printf("\n");
	printf("****serveur suivant est:****\n");
	afficherIdentConnexion(SERVEUR.suivServeur);
	printf("\n");
#endif
	return 1;
}







//coupe la socket passé en arg et indique au serveur de couper la communication
int message_disconnect_from_server(socket_t sockClient){

	envoyerOrigine(FROM_SERVEUR,sockClient);
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




