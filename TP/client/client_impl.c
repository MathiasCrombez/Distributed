#include"client.h"


/** variable global **/
client_t CLIENT;


#define GETSOCKET() CLIENT.idSocket



client_t creerClient(const char *nom, uint32_t k)
{

	//client_t client;
	//struct hostent *hostinfo;

	//initialisation des champs du client
	CLIENT.monNom = nom;
	CLIENT.port = PORT;
	CLIENT.cle = k;
	CLIENT.idSocket = socket(AF_INET, SOCK_STREAM, 0);
	//printf("socket %d\n", CLIENT.idSocket);
	if (CLIENT.idSocket < 0) {
		perror("socket()");
		exit(-1);
	}

/*	//assert(hostinfo=gethostbyname(SERVEURNAME)!= NULL);
	hostinfo = gethostbyname(SERVEURNAME);
	if (hostinfo == NULL) {
		printf("Récupération du serveur impossible\n");
		exit(-1);
	}*/

	CLIENT.serv_addr.sin_family = AF_INET;
	CLIENT.serv_addr.sin_port = htons(PORT);
	CLIENT.serv_addr.sin_addr.s_addr = inet_addr(SERVEURNAME);

	return CLIENT;
}

uint32_t connect2server(client_t client)
{

	if (connect
	    (client.idSocket, (struct sockaddr *)&client.serv_addr,
	     sizeof(struct sockaddr_in)) < 0) {
			printf("Echec de la connexion\n");
			perror("connect()");
			exit(-1);
	}
	return 0;
}

uint32_t envoyerMessage(message_t message, socket_t destination)
{
	return (uint32_t) send(destination, &message, sizeof(message), 0);
}


uint32_t put(uint32_t cle, uint32_t valeur)
{
	if (write(GETSOCKET(), &cle, sizeof(uint32_t)) < 0) {
		printf("PUT : echec de l'envoi de la clé %d\n", cle);
		return -1;
	}
	if (write(GETSOCKET(), &valeur, sizeof(uint32_t)) < 0) {
		printf
		    ("PUT : echec de l'envoi de la valeur %d associé à la clé %d \n",
		     cle, valeur);
		return -1;
	}

	return 0;
}

uint32_t get(uint32_t cle)
{
	uint32_t valeur = 0;

	if (write(GETSOCKET(), &cle, sizeof(uint32_t)) < 0) {
		printf("GET : echec de l'envoi de la clé %d\n", cle);
		return -1;
	}
	if (read(GETSOCKET(), &valeur, sizeof(uint32_t)) < 0) {
		printf("GET : echec de la lecture de la valeur");
		return -1;
	}
	return valeur;
}

uint32_t removeKey(uint32_t cle)
{
	if (write(GETSOCKET(), &cle, sizeof(uint32_t)) < 0) {
		printf("REMOVE : echec de l'envoi de la clé %d\n", cle);
		return -1;
	}
	return 0;
}



uint32_t disconnect2server()
{
	// signal de mort?
	if (shutdown(GETSOCKET(), 2) < 0) {
		printf("disconnect2server : echec de la deconnexion\n");
	}
	return 0;
}

