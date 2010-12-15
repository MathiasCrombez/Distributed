#include "serveur_impl.h"

#define SET_SERVEUR_NAME(NAME,PORT){				\
	strcpy(SERVEUR.name,NAME);				\
	strcat(SERVEUR.name,":");				\
	sprintf(SERVEUR.name+strlen(NAME)+1,"%llu",PORT);        \
}


/*
 *Initialisation de la variable globale SERVEUR.
 */
serveur_t* creerServeur(char *nomDuServeur, uint64_t port)
{

	int yes = 1;

//TODO  //## MODIFIER LE CHAMP NOM:METTRE UN POINTEUR######//
	//## SOURCE DE SEGFAULt SI LE NOM EST TROP GRAND###//
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

	SERVEUR.suivServeur =(struct idConnexion *)malloc(sizeof(struct idConnexion));
	if (SERVEUR.suivServeur == NULL) {
            perror("malloc()");
            exit(EXIT_FAILURE);
	}

	SERVEUR.precServeur =
	    (struct idConnexion *)malloc(sizeof(struct idConnexion));
	if (SERVEUR.precServeur == NULL) {
            perror("malloc()");
            exit(EXIT_FAILURE);
	}

	SERVEUR.suivServeur->identifiant = SERVEUR.serv_addr;
	SERVEUR.suivServeur->name = SERVEUR.name;
	SERVEUR.suivServeur->h=0;
	SERVEUR.suivServeur->taille_hashtab=0;
	
	SERVEUR.precServeur->identifiant = SERVEUR.serv_addr;
	SERVEUR.precServeur->name = SERVEUR.name;
	SERVEUR.precServeur->h=0;
	SERVEUR.precServeur->taille_hashtab=0;

/*	SERVEUR.firstKey = 0;*/
/*	SERVEUR.nextKey = 0;*/
/*	SERVEUR.precKey = 0;*/

	// evite le message d'erreur "Address already in use" lors d'un bind
	if (setsockopt (SERVEUR.idSocket, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
		perror("setsockopt()");
		exit(EXIT_FAILURE);
	}

	if (bind(SERVEUR.idSocket, (struct sockaddr *)&SERVEUR.serv_addr, sizeof(SERVEUR.serv_addr)) < 0) {
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	if (listen(SERVEUR.idSocket, LENGTH_LISTEN_QUEUE)) {
		perror("liste()");
		exit(EXIT_FAILURE);
	}
	return &SERVEUR;
}

/**
 *A chaque connection d'un client, un thread est créé et talk_to_client est appelée.
 */
void *talk_to_client(void *idSocket)
{
    cle_t K;
    donnee_t D;
    requete_t type_requete;
    socket_t sockClient = *(socket_t*) idSocket;
    tabClient_t curr, prev;
    uint64_t h;
    while(1) {
        recevoirTypeMessage(&type_requete, sockClient);
        switch (type_requete) {
            
        case PUT:
            recevoirCle(&K,sockClient);
            h = hash(K) % MAX_TAILLE_HASH_TABLE;
            printf("talk_to_client:PUT:cle(%s), hash(%llu)\n",K, h);
            if ( ! (SERVEUR.h <= h && h <= (SERVEUR.h + SERVEUR.tabl.taille))){
                envoyerOctet(0,sockClient);
                envoyerIdent(SERVEUR.suivServeur, sockClient);
            } else {
                envoyerOctet(1,sockClient);
                recevoirDonnee(&D,sockClient);
                putHashTable(D,SERVEUR.tabl);
            }
            break;

        case GET:
            recevoirCle(&K,sockClient);
            h = hash(K) % MAX_TAILLE_HASH_TABLE;
            printf("talk_to_client:GET:cle(%s), hash(%llu)\n",K, h);
            if ( ! (SERVEUR.h <= h && h <= (SERVEUR.h + SERVEUR.tabl.taille))){
                envoyerOctet(0,sockClient);
                envoyerIdent(SERVEUR.suivServeur, sockClient);
            } else {
                D = getHashTable(K,SERVEUR.tabl); 
                envoyerOctet(1,sockClient);
                if (D==NULL) {
                    envoyerOctet(0,sockClient);
                } else {
                    envoyerOctet(1,sockClient);
                    envoyerDonnee(D,sockClient);
                }
            }
            break;

        case REMOVEKEY:
            recevoirCle(&K,sockClient);
            h = hash(K) % MAX_TAILLE_HASH_TABLE;
            printf("talk_to_client:cle(%s), hash(%llu)\n",K, h);
            if ( ! (SERVEUR.h <= h && h <= (SERVEUR.h + SERVEUR.tabl.taille))){
                envoyerOctet(0,sockClient);
                envoyerIdent(SERVEUR.suivServeur, sockClient);
            } else {
                D = getHashTable(K,SERVEUR.tabl); 
                envoyerOctet(1,sockClient);
                if (D==NULL) {
                    envoyerOctet(0,sockClient);
                } else {
                    valeur_t V = removeHashTable(K,SERVEUR.tabl);
                    envoyerOctet(1,sockClient);
                    envoyerValeur(V, sockClient);
                    free(V);
                }
            }
            break;
        case IDENT:
            break;

        case CONNECT:
            envoyerOctet(1,sockClient);
            break;
            
        case DISCONNECT:
            curr = SERVEUR.tableauClient;
            prev = SERVEUR.tableauClient;
            if (curr->client.idSocket == sockClient) {
                SERVEUR.tableauClient = curr->suiv;
                free(curr);
            }
            else {
                while(curr->client.idSocket != sockClient && curr) {
                    prev = curr;
                    curr = curr->suiv;                    
                }
                if (curr) {
                    prev->suiv = curr->suiv;
                    free(curr);
                }
                else {
                    printf("talk_to_client:DISCONNECT:client de socket %d inconnu.\n"
                           , sockClient);
                    close(sockClient);
                    pthread_exit(NULL);
                }
            }
            close(sockClient);
            pthread_exit(NULL);
            break;
	default:
            break;

        }
    }
}
/**
* Corps de la fonction de routine lors de la création de pthread
* (apparition d'un client
*/
void *talk_to_server(void *idSocket)
{
	//ce socket va nous permettre de communiquer avec le client
	socket_t sockServer = *(socket_t*)idSocket;
	char reponse;
	char *nom;
	struct idConnexion id_connexion;
	requete_t type_requete;
	
#ifdef DEBUG_SERVEUR_IMPL
	printf("talk_to_server:Debut\n");
#endif
	
	recevoirTypeMessage(&type_requete, sockServer);
	switch (type_requete) {
	

	case CONNECT:
	
		printf("talk_to_server:CONNECT\n");
		recevoirOctet(&reponse, sockServer);

		if (reponse == 0) {
			id_connexion = get_my_idConnexion();
			envoyerIdent(&id_connexion, sockServer);
			envoyerIdent(SERVEUR.suivServeur,sockServer);
			recevoirIdent(&SERVEUR.suivServeur,sockServer);
			
		} else if (reponse == 1) {
			recevoirIdent(&SERVEUR.precServeur,sockServer);
		}

	#ifdef DEBUG_SERVEUR_IMPL
		printf("talk_to_server:Serveur precedent:\n");
		afficherIdentConnexion(SERVEUR.precServeur);
		printf("\n");
		printf("talk_to_server:Serveur suivant:\n");
		afficherIdentConnexion(SERVEUR.suivServeur);
		printf("\n");
	#endif
		
		break;

	case IDENT:
	
            printf("talk_to_server:IDENT\n");
            id_connexion= get_my_idConnexion();
            printf("talk_to_server:Taille dht(%u)\n",id_connexion.taille_hashtab);
            envoyerIdent(&id_connexion,sockServer);
            break;
		
		
	case WHOIS_NEXT_SERVER:
	
		printf("talk_to_server:WHOIS_NEXT_SERVER\n");
		envoyerIdent(SERVEUR.suivServeur,sockServer);
		break;
		
		
/*	case DISCONNECT:*/

/*		printf("un serveur souhaite ma mort\n");*/
/*		recevoirIdent(&(SERVEUR.precServeur->identifiant), sockServer);*/
/*		recevoirChaine(&(SERVEUR.precServeur->name), sockServer);*/
/*		printf("c'est fait!\n");*/
/*		return NULL;*/
	default:
		printf("talk_to_serveur:Message inconnu");
		break;

	}
	
	/* fermeture de la communication et mort ddu thread*/
	
	printf("talk_to_server:End\n");
	shutdown(sockServer, SHUT_RDWR);
	pthread_exit(NULL);
}




idConnexion_t get_my_idConnexion()
{
	idConnexion_t  id_connexion;
	
	id_connexion.identifiant=SERVEUR.serv_addr;
	id_connexion.name = SERVEUR.name;	
	id_connexion.h=SERVEUR.h;
	id_connexion.taille_hashtab= SERVEUR.tabl.taille;
	
	return id_connexion;
}


serveur_t* get_my_server()
{
	return &SERVEUR;
}
