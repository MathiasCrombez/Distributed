#include"client_impl.h"





client_t creerClient(const char *nom)
{
    client_t client;
	/**initialisation des champs du client**/
    client.monNom = nom;
    return client;
}

idConnexion_t * preConnect(char *to_serveur, uint64_t port) {
	char *hostname = to_serveur;
	struct hostent *hostinfo = NULL;
        idConnexion_t * server;
        server = malloc(sizeof(idConnexion_t));

	hostinfo = gethostbyname(hostname);
	if (hostinfo == NULL) {
		printf("preConnect:erreur gethostbyname()\n", to_serveur);
		exit(-1);
	}

	server->identifiant.sin_family = AF_INET;
	server->identifiant.sin_port = htons(port);
	server->identifiant.sin_addr = *(struct in_addr *)hostinfo->h_addr;
        server->name = to_serveur;

        return server;
}

/* socket_t connect2server(char *to_serveur, uint64_t port) */
/* { */
/* 	char *hostname = to_serveur; */
/* 	struct hostent *hostinfo = NULL; */

/* 	hostinfo = gethostbyname(hostname); */
/* 	if (hostinfo == NULL) { */
/* 		printf("erreur gethostbyname():le serveur %s est inconnu\n", to_serveur); */
/* 		exit(-1); */
/* 	} */

/* 	CLIENT.serv_addr.sin_family = AF_INET; */
/* 	CLIENT.serv_addr.sin_port = htons(port); */
/* 	CLIENT.serv_addr.sin_addr = *(struct in_addr *)hostinfo->h_addr; */
	
/* 	CLIENT.idSocket = socket(AF_INET, SOCK_STREAM, 0); */
/* 	if (CLIENT.idSocket < 0) { */
/* 		perror("socket()"); */
/* 		exit(-1); */
/* 	} */
	
/* 	if (connect(CLIENT.idSocket, (struct sockaddr *)&CLIENT.serv_addr,sizeof(struct sockaddr_in)) < 0) { */
/* 	#ifdef DEBUG_CLIENT_IMPL */
/* 		printf("Echec de la connexion\n"); */
/* 	#endif */
/* 		exit(-1); */
/* 	} */
/* 	return CLIENT.idSocket; */
/* } */


socket_t connect2server(idConnexion_t *server, client_t client)
{
	client.serv_addr.sin_family = server->identifiant.sin_family;
	client.serv_addr.sin_port = server->identifiant.sin_port;
	client.serv_addr.sin_addr = server->identifiant.sin_addr;
	
	client.idSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (client.idSocket < 0) {
		perror("socket()");
		exit(-1);
	}
	
	if (connect(client.idSocket, (struct sockaddr *)&client.serv_addr,sizeof(struct sockaddr_in)) < 0) {
            //	#ifdef DEBUG_CLIENT_IMPL
		printf("connect2server:Echec de la connexion\n");
                //	#endif
		exit(-1);
	}
	return client.idSocket;
}


