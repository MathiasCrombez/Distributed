#include "serveur_impl.h"

#define SET_SERVEUR_NAME(NAME,PORT){				\
	strcpy(SERVEUR.name,NAME);				\
	strcat(SERVEUR.name,":");				\
	sprintf(SERVEUR.name+strlen(NAME)+1,"%ld",PORT);        \
}


///###### METTRE TON IP ICI ######//
#define SERVEURNAME "129.88.57.45"


/**
*Initialisation de la variable globale SERVEUR.
**/
serveur_t creerServeur(char *nomDuServeur, uint64_t port)
{
	int yes = 1;

//TODO	//## MODIFIER LE CHAMP NOM:METTRE UN POINTEUR######
	SET_SERVEUR_NAME(nomDuServeur, port);
	SERVEUR.idSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (SERVEUR.idSocket < 0) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	
	//initialisation de la structure serveur
	SERVEUR.serv_addr.sin_family = AF_INET;
	//SERVEUR.serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	SERVEUR.serv_addr.sin_addr.s_addr = inet_addr(SERVEURNAME);
	SERVEUR.serv_addr.sin_port = htons(port);
	
	SERVEUR.suivServeur = (struct idConnexion*)malloc(sizeof(struct idConnexion));
	if(SERVEUR.suivServeur==NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	
	SERVEUR.precServeur = (struct idConnexion*)malloc(sizeof(struct idConnexion));
	if(SERVEUR.precServeur==NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	
	SERVEUR.suivServeur->identifiant = SERVEUR.serv_addr;
	SERVEUR.suivServeur->name = SERVEUR.name;
	SERVEUR.suivServeur->idSocket= 0;
	
	SERVEUR.precServeur->identifiant = SERVEUR.serv_addr;
	SERVEUR.precServeur->name = SERVEUR.name;
	SERVEUR.precServeur->idSocket = 0;
	
	SERVEUR.firstKey = 0;
	SERVEUR.nextKey  = 0;
	SERVEUR.precKey  = 0;
	
	// evite le message d'erreur "Address already in use" lors d'un bind
	if (setsockopt(SERVEUR.idSocket, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
		perror("setsockopt()");
		exit(EXIT_FAILURE);
	}

	if (bind(SERVEUR.idSocket, (struct sockaddr *)&SERVEUR.serv_addr,sizeof(SERVEUR.serv_addr)) < 0) {
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	if (listen(SERVEUR.idSocket, LENGTH_LISTEN_QUEUE)) {
		perror("liste()");
		exit(EXIT_FAILURE);
	}
	return SERVEUR;
}



/**
 *A chaque connection d'un client, un thread est créé et talk_to_client est appelée.
 */
void* talk_to_client(void* idSocket)
{
	//ce socket va nous permettre de communiquer avec le client
	
	socket_t sockClient = (socket_t)idSocket;

	/**
     	* Corps de la fonction de routine lors de la création de pthread
     	* (apparition d'un client
     	*/
	requete_t type_requete;
	recevoirTypeMessage(&type_requete, sockClient);

	switch (type_requete) {

	case PUT:
		break;

	case GET:
		break;

	case ACK:
		break;

	case IDENT:
		break;

	case CONNECT:

		SERVEUR.suivServeur->idSocket = sockClient;
		printf("un serveur veut participer à la dht\n");
		
		envoyerIdent(SERVEUR.serv_addr, sockClient);	
		envoyerChaine(SERVEUR.name, sockClient);
		
		//envoi des champs de suivServeur
		envoyerIdent(SERVEUR.suivServeur->identifiant, sockClient);	
		envoyerChaine(SERVEUR.suivServeur->name, sockClient);
		
		//envoi de firstKey et nextKey
		envoyerHash(SERVEUR.firstKey, sockClient);
		envoyerHash(SERVEUR.nextKey, sockClient);
		
		recevoirIdent(&(SERVEUR.suivServeur->identifiant), sockClient);
		recevoirChaine(&(SERVEUR.suivServeur->name), sockClient);
		
		recevoirHash(&(SERVEUR.nextKey), sockClient);
		
		
		//enregistrement de la connection
		//shutdown(SERVEUR.suivServeur->idSocket,2);
		
//TODO       //#### decrementer le compteur nbclient
		
		//#### debug ####//
		printf("****serveur precedent est:****\n");
		afficherIdentConnexion(SERVEUR.precServeur);
		printf("\n");
		printf("****serveur suivant est:****\n");
		afficherIdentConnexion(SERVEUR.suivServeur);
		printf("\n");
		//#### debug ####//
		break;


	case DISCONNECT:
	
		printf("un serveur souhaite ma mort\n");
		shutdown(sockClient,SHUT_RDWR);
		printf("c'est fait!\n");
		break;
	default:
		break;

	}

}

socket_t connect2server(char *to_serveur, uint64_t port)
{

	struct hostent *hostinfo = NULL;
	struct sockaddr_in server_info;
	socket_t idSocket;


	
	hostinfo = gethostbyname(to_serveur);
	if (hostinfo == NULL) {
		printf("erreur gethostbyname():le serveur %s est inconnu\n",to_serveur);
		return 0;
	}

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(port);
	server_info.sin_addr = *(struct in_addr *)hostinfo->h_addr;
	
	idSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (idSocket < 0) {
		perror("socket()");
		return 0;
	}

	if (connect(idSocket, (struct sockaddr *)&server_info,sizeof(struct sockaddr_in)) == -1) {
		perror("connect()");
		return 0;
	}
	
	
	return idSocket;
}

    /*
     * Fermeture de la socket
     */

   // shutdown(sockClient, 2);

    /*
     * On peut utiliser pthread_exit pour renvoyer une valeur (d'erreur ou autre) 
     * => voir pthread_join
     */
    //pthread_exit(NULL);

/*int put_h(serveur_t s, cle_t cle, valeur_t valeur, uint64_t taille)*/
/*{*/
/*	char str[taille];*/
/*	hash_t *curr, *prec, *newCell;*/
/*	int i;*/
/*	uint64_t key_l = cle - s.firstKey;*/
/*	newCell =*/
/*	    malloc(sizeof(char) * taille + sizeof(taille) + sizeof(hash_t *));*/
/*	for (i = 0; i < taille; i++) {*/
/*		str[i] = valeur[i];*/
/*	}*/
/*	prec = s.tabl[key_l];*/
/*	curr = prec;*/
/*	while (curr != NULL) {*/
/*		prec = curr;*/
/*		curr = curr->suiv;*/
/*	}*/
/*	newCell->size = taille;*/
/*	newCell->value = str;*/
/*	newCell->suiv = NULL;*/
/*	return 0;*/
/*}*/

/*uint64_t get_h(serveur_t s, uint64_t cle, char *valeur)*/
/*{*/
/*	uint64_t key_l = cle - s.firstKey;*/
/*	hash_t *curr = s.tabl[key_l];*/
/*	while (curr != NULL && strcmp(curr->value, valeur)) {*/
/*		curr = curr->suiv;*/
/*	}*/
/*	if (curr == NULL) {*/
/*		valeur = NULL;*/
/*		return 0;*/
/*	} else {*/
/*		valeur = curr->value;*/
/*		return curr->size;*/
/*	}*/
/*}*/

/*int remove_h(serveur_t s, uint64_t cle, char *valeur, uint64_t taille)*/
/*{*/
/*	int key_l = cle - s.firstKey;*/
/*	hash_t *curr, *prec;*/
/*	curr = s.tabl[key_l];*/
/*	if (curr != NULL && curr->size == taille*/
/*	    && !strcmp(curr->value, valeur)) {*/
/*		s.tabl[key_l] = curr->suiv;*/
/*		free(curr);*/
/*		return 0;*/
/*	} else {*/
/*		while (curr != NULL && curr->size != taille*/
/*		       && !strcmp(curr->value, valeur)) {*/
/*			prec = curr;*/
/*			curr = curr->suiv;*/
/*		}*/
/*		if (curr == NULL) {*/
/*			return -1;*/
/*		} else {*/
/*			prec->suiv = curr->suiv;*/
/*			free(curr);*/
/*			return 0;*/
/*		}*/
/*	}*/
/*}*/

