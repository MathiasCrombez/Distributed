#include "serveur_impl.h"

#define SET_SERVEUR_NAME(NAME,PORT)                             \
    do{                                                         \
        strcpy(SERVEUR.name,NAME);                              \
        strcat(SERVEUR.name,":");                               \
        sprintf(SERVEUR.name+strlen(NAME)+1,"%llu",PORT);       \
    }while(0)

/*
 *Initialisation de la variable globale SERVEUR.
 */
serveur_t *creerServeur(char *nomDuServeur, uint64_t port)
{

	int yes = 1;

	SET_SERVEUR_NAME(nomDuServeur, port);
	SERVEUR.idSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (SERVEUR.idSocket < 0) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	//initialisation de la structure serveur
	SERVEUR.serv_addr.sin_family = AF_INET;
	SERVEUR.serv_addr.sin_addr.s_addr = inet_addr(SERVEURNAME);
	SERVEUR.serv_addr.sin_port = htons(port);

	SERVEUR.suivServeur = SERVEUR.serv_addr;
    SERVEUR.precServeur = SERVEUR.serv_addr;
	// evite le message d'erreur "Address already in use" lors d'un bind
	if (setsockopt
	    (SERVEUR.idSocket, SOL_SOCKET, SO_REUSEADDR, &yes,
	     sizeof(int)) == -1) {
		perror("setsockopt()");
		exit(EXIT_FAILURE);
	}

	if (bind
	    (SERVEUR.idSocket, (struct sockaddr *)&SERVEUR.serv_addr,
	     sizeof(SERVEUR.serv_addr)) < 0) {
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	if (listen(SERVEUR.idSocket, 5)) {
		perror("listen()");
		exit(EXIT_FAILURE);
	}
	return &SERVEUR;
}

pthread_t preconnect_serv2cli(struct sockaddr_in cli_addr, socket_t sockClient,
			      serveur_t ** serveur_ptr)
{
	pthread_t client_thread;
	tabClient_t p;

	p = malloc(sizeof(struct tableauClient));
	p->client.identifiant = cli_addr;
	p->client.idSocket = sockClient;

	client_thread = p->client.thread;

	p->suiv = (*serveur_ptr)->tableauClient;
	(*serveur_ptr)->tableauClient = p;
	return client_thread;

}

/**
 *A chaque connection d'un client, un thread est créé et talk_to_client est appelée.
 */
void *talk_to_client(void *idSocket)
{
	cle_t K;
	donnee_t D;
	requete_t type_requete;
	socket_t sockClient = (socket_t) idSocket;
	tabClient_t curr, prev;
	uint64_t h;

	while (1) {

		pthread_mutex_lock(&MUTEX_NB_JOBS);
		NB_JOBS++;
		pthread_mutex_unlock(&MUTEX_NB_JOBS);

		recevoirTypeMessage(&type_requete, sockClient);
#ifdef DEBUG_SERVEUR_IMPL
		printf("%d:reçu %d\n", sockClient, type_requete);
#endif
		switch (type_requete) {

		case PUT:
			pthread_cond_signal(&condition_cond);
			recevoirCle(&K, sockClient);

			h = hash(K);
#ifdef DEBUG_SERVEUR_IMPL
			printf("talk_to_client:PUT:cle(%s), hash(%llu)\n", K,
			       h);
#endif

			if (SERVEUR.h <= h
			    && h < (SERVEUR.h + SERVEUR.tabl.taille)) {
				envoyerOctet(1, sockClient);
				recevoirDonnee(&D, sockClient);
				putHashTable(D, SERVEUR.tabl);
			} else {
				envoyerOctet(0, sockClient);
				envoyerSockAddr(SERVEUR.suivServeur,
						sockClient);
			}

			//pour les testes
			printf("put terminé\n");
			break;

		case GET:
			pthread_cond_signal(&condition_cond);
			recevoirCle(&K, sockClient);

			h = hash(K);
#ifdef DEBUG_SERVEUR_IMPL
			printf("talk_to_client:GET:cle(%s), hash(%llu)\n", K,
			       h);
#endif
			if (SERVEUR.h <= h
			    && h < (SERVEUR.h + SERVEUR.tabl.taille)) {
				D = getHashTable(K, SERVEUR.tabl);
				envoyerOctet(1, sockClient);
				if (D == NULL) {
					envoyerOctet(0, sockClient);
				} else {
					envoyerOctet(1, sockClient);
					envoyerDonnee(D, sockClient);
				}
			} else {

				envoyerOctet(0, sockClient);
				envoyerSockAddr(SERVEUR.suivServeur,
						sockClient);
			}
			break;

		case REMOVEKEY:
			pthread_cond_signal(&condition_cond);
			recevoirCle(&K, sockClient);

			h = hash(K);
#ifdef DEBUG_SERVEUR_IMPL
			printf("talk_to_client:cle(%s), hash(%llu)\n", K, h);
#endif
			if (SERVEUR.h <= h
			    && h < (SERVEUR.h + SERVEUR.tabl.taille)) {

				D = getHashTable(K, SERVEUR.tabl);
				envoyerOctet(1, sockClient);
				if (D == NULL) {
					envoyerOctet(0, sockClient);
				} else {
					valeur_t V =
					    removeHashTable(K, SERVEUR.tabl);
					envoyerOctet(1, sockClient);
					envoyerValeur(V, sockClient);
					free(V);
				}
			} else {

				envoyerOctet(0, sockClient);
				envoyerSockAddr(SERVEUR.suivServeur,sockClient);
			}
			break;
		

		case CONNECT:
			pthread_cond_signal(&condition_cond);
			//envoyerOctet(1,sockClient);

			break;

		case QUIT:

			/*pthread_cond_signal(&condition_cond);!!! */
			printf("QUIT\n");

/*			pthread_mutex_lock(&SERVER_IS_DYING);*/
/*			if (SERVER_IS_DYING_VAR == 0) {*/
/*				SERVER_IS_DYING_VAR = 1;*/
/*				//envoyer octet 1;*/
/*			} else {*/
/*				printf("serveur is already dead");*/
/*				//envoyerOctet 0*/
/*			}*/
/*			pthread_mutex_unlock(&SERVER_IS_DYING);*/

/*			if (NB_JOBS != 1) {*/
/*				printf*/
/*				    ("en attente que les requetes soient satisfaites\n");*/
/*				pthread_cond_wait(&COND_NB_JOBS,*/
/*						  &MUTEX_NB_JOBS);*/
/*			}*/
			printf("le serveur est libre\n");
			message_quit();
			exit(0);
			break;

		case DISCONNECT:
			pthread_cond_signal(&condition_cond);
			printf("disconnect\n");
/*            curr = SERVEUR.tableauClient;*/
/*            prev = SERVEUR.tableauClient;*/
/*            if (curr->client.idSocket == sockClient) {*/
/*                SERVEUR.tableauClient = curr->suiv;*/
/*                free(curr);*/
/*            }*/
/*            else {*/
/*                while(curr->client.idSocket != sockClient && curr) {*/
/*                    prev = curr;*/
/*                    curr = curr->suiv;                    */
/*                }*/
/*                if (curr) {*/
/*                    prev->suiv = curr->suiv;*/
/*                    free(curr);*/
/*                }*/
/*                else {*/
/*                    printf("talk_to_client:DISCONNECT:client de socket %d inconnu.\n"*/
/*                           , sockClient);*/
/*                    close(sockClient);*/
/*                    pthread_exit(NULL);*/
/*                }*/
/*            }*/
			shutdown(sockClient, SHUT_RDWR);

			pthread_exit(NULL);

			break;

		case STATUS:

			printf("STATUS\n");
			idConnexion_t ident;
			ident = get_my_idConnexion();
			envoyerIdent(ident, sockClient);
			break;

			break;

		default:
			printf("Message inconnu\n");
			break;

		}

		pthread_mutex_lock(&MUTEX_NB_JOBS);
		NB_JOBS--;
		if (NB_JOBS == 0) {
			pthread_cond_signal(&COND_NB_JOBS);
		}
		pthread_mutex_unlock(&MUTEX_NB_JOBS);

	}
}

/**
 * Corps de la fonction de routine lors de la création de pthread
 * (apparition d'un client
 */
void *talk_to_server(void *idSocket)
{

	//ce socket va nous permettre de communiquer avec le client
	socket_t sockServer = (socket_t) idSocket;
	requete_t type_requete;
	idConnexion_t id_connexion;
	uint64_t h;
#ifdef DEBUG_SERVEUR_IMPL
	printf("########debut du thread#########\n");
#endif
	pthread_mutex_lock(&MUTEX_NB_JOBS);
	NB_JOBS++;
	pthread_mutex_unlock(&MUTEX_NB_JOBS);
	recevoirTypeMessage(&type_requete, sockServer);
	char reponse;
	
	switch (type_requete) {

	case CONNECT:

		printf("SERVER CONNECT\n");
		recevoirOctet(&reponse,sockServer);
		
		if(reponse==0){
		    envoyerSockAddr(SERVEUR.suivServeur, sockServer);
		    recevoirSockAddr(&SERVEUR.suivServeur, sockServer);
        }
        else if(reponse==1){
            recevoirSockAddr(&SERVEUR.precServeur, sockServer);
        }
        else {
            printf("erreur connect\n");
            exit(0);
        }


		break;

	case IDENT:

		printf("IDENT\n");
		id_connexion = get_my_idConnexion();
		afficherIdentConnexion(id_connexion);
		envoyerIdent(id_connexion, sockServer);
		break;

	case WHOIS_NEXT_SERVER:

		printf("WHOIS_NEXT_SERVER\n");
		envoyerSockAddr(SERVEUR.suivServeur, sockServer);
		break;

		/* un serveur demande de partager la table de hachage */
	case RECEIVE_DHT:

		printf("TRANSFER DHT\n");
		table_de_hachage_t my_hashtab = get_my_hashtab();
		liste_t L;
		uint64_t i;
		donnee_t D;

		recevoirHash(&h, sockServer);

		for (i = h - SERVEUR.h; i < my_hashtab.taille; i++) {

			L = my_hashtab.table_de_hachage[i];

			while (L != NULL) {
				D = removeTeteDeListe(&L);
				assert(D != NULL);	//aucune donné ne devrait etre nul
				envoyerOctet(1, sockServer);
				envoyerDonnee(D, sockServer);
				libererDonnee(D);
			}
		}
		//fin d'envoi
		envoyerOctet(0, sockServer);

		reallocHashTable(&SERVEUR.tabl, SERVEUR.tabl.taille / 2,0);
		afficherInfoHashTable();
		afficherHashTable(SERVEUR.tabl);
		break;
/*        case DISCONNECT:*/

/*                printf("un serveur souhaite ma mort\n");*/
/*                recevoirIdent(&(SERVEUR.precServeur->identifiant), sockServer);*/
/*                recevoirChaine(&(SERVEUR.precServeur->name), sockServer);*/
/*                printf("c'est fait!\n");*/
/*                return NULL;*/

	case TRANSFER_DHT:

		printf("TRANSFER_DHT\n");
		uint32_t taille_hashtab;
		char reponse;
		uint32_t new_size;

		recevoirUInt_32(&taille_hashtab, sockServer);
		recevoirUInt_64(&h, sockServer);
		
		new_size = taille_hashtab + SERVEUR.tabl.taille;
		assert(new_size <= MAX_TAILLE_HASH_TABLE);

		recevoirOctet(&reponse, sockServer);
		reallocHashTable(&SERVEUR.tabl, new_size, SERVEUR.h);

		while (reponse) {
			recevoirDonnee(&D, sockServer);
			afficherDonnee(D);
			putHashTable(D, SERVEUR.tabl);
			recevoirOctet(&reponse, sockServer);
		}

		//changer mon hash prendre le mininmin
		SERVEUR.h = (h < SERVEUR.h) ? h : SERVEUR.h;

		afficherMyIdConnexion();
		break;
		
		
	case QUIT:
		
	    printf("SERVER QUIT\n");
		recevoirOctet(&reponse,sockServer);
		
		if(reponse==0){
		    recevoirSockAddr(&SERVEUR.suivServeur, sockServer);
        }
        else if(reponse==1){
            recevoirSockAddr(&SERVEUR.precServeur, sockServer);
        }
        else {
            printf("erreur connect\n");
            exit(0);
        }
	
	    break;
	    
	default:
		printf("message inconnu");
		break;

	}

	/* fermeture de la communication et mort ddu thread */

	pthread_mutex_lock(&MUTEX_NB_JOBS);
	NB_JOBS--;
	if (NB_JOBS == 0) {
		pthread_cond_signal(&COND_NB_JOBS);
	}
	pthread_mutex_unlock(&MUTEX_NB_JOBS);

	printf("########fin du thread#########\n");
	close(sockServer);
	pthread_exit(NULL);
}

idConnexion_t get_my_idConnexion()
{
	return setIdConnexion(SERVEUR.name,
			      SERVEUR.serv_addr,
			      SERVEUR.h,
			      SERVEUR.tabl.taille,
			      SERVEUR.suivServeur,
			      SERVEUR.precServeur);
}

serveur_t *get_my_server()
{
	return &SERVEUR;
}

table_de_hachage_t get_my_hashtab()
{
	return SERVEUR.tabl;
}

/*
 * methode de debug
 */

void afficherInfoHashTable()
{

	printf("Info sur hash table\n");

	printf("****\n\tma taille: %d\n", SERVEUR.tabl.taille);
	printf("\tmon h: %lld\n", SERVEUR.h);
}

void afficherMyIdConnexion()
{

	afficherIdentConnexion(get_my_idConnexion());

}

