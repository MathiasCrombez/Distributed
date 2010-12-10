#include"client.h"



#define IDSOCKET CLIENT.idSocket


/**
 * Création de la structure client
 */
client_t creerClient(const char *nom)
{
    client_t client;
	
    /**initialisation des champs du client**/
    client.monNom = nom;
    client.idSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (client.idSocket < 0) {
        perror("socket()");
        exit(-1);
    }
	
    return client;
}
/**
 * Se connecte au serveur donné
 */
uint32_t connect2server(client_t client, char* to_serveur, uint64_t port)
{
    char* hostname= to_serveur;	
    struct hostent *hostinfo = NULL;
		
    hostinfo = gethostbyname(hostname);
    if (hostinfo == NULL) {
        printf("erreur gethostbyname():le serveur %s est inconnu\n", to_serveur);
        return -1;
    }

    client.serv_addr.sin_family = AF_INET;
    client.serv_addr.sin_port = htons(port);
    client.serv_addr.sin_addr = *(struct in_addr *) hostinfo->h_addr;
	
    if (connect(client.idSocket, (struct sockaddr *)&client.serv_addr,
                sizeof(struct sockaddr_in)) < 0) {
#ifdef DEBUG_MESSAGE
        printf("Echec de la connexion\n");
#endif
        return -1;
    }
    return 0;
}

/*uint32_t put(uint32_t cle, uint32_t valeur)*/
/*{*/
/*	if (write(IDSOCKET, &cle, sizeof(uint32_t)) < 0) {*/
/*		printf("PUT : echec de l'envoi de la clé %d\n", cle);*/
/*		return -1;*/
/*	}*/
/*	if (write(IDSOCKET, &valeur, sizeof(uint32_t)) < 0) {*/
/*		printf*/
/*		    ("PUT : echec de l'envoi de la valeur %d associé à la clé %d \n",*/
/*		     cle, valeur);*/
/*		return -1;*/
/*	}*/

/*	return 0;*/
/*}*/

/*uint32_t get(uint32_t cle)*/
/*{*/
/*	uint32_t valeur = 0;*/

/*	if (write(IDSOCKET, &cle, sizeof(uint32_t)) < 0) {*/
/*		printf("GET : echec de l'envoi de la clé %d\n", cle);*/
/*		return -1;*/
/*	}*/
/*	if (read(IDSOCKET, &valeur, sizeof(uint32_t)) < 0) {*/
/*		printf("GET : echec de la lecture de la valeur");*/
/*		return -1;*/
/*	}*/
/*	return valeur;*/
/*}*/

/*uint32_t removeKey(uint32_t cle)*/
/*{*/
/*	if (write(IDSOCKET, &cle, sizeof(uint32_t)) < 0) {*/
/*		printf("REMOVE : echec de l'envoi de la clé %d\n", cle);*/
/*		return -1;*/
/*	}*/
/*	return 0;*/
/*}*/

/*uint32_t disconnect2server()*/
/*{*/
/*	// signal de mort?*/
/*	if (shutdown(IDSOCKET, 2) < 0) {*/
/*		printf("disconnect2server : echec de la deconnexion\n");*/
/*	}*/
/*	return 0;*/
/*}*/

/*uint32_t quit(char *nomDuServeur)*/
/*{*/

/*	message_t message;*/

/*	message.type = QUIT;*/
/*	memcpy(message.requete.quit.nomDuServeur, nomDuServeur, MAXCAR);*/

/*	envoyerMessage(message, IDSOCKET);*/
/*	recevoirMessage(message, IDSOCKET);*/

/*}*/

