#include "serveur_impl.h"

#define SET_SERVEUR_NAME(NAME,PORT){										\
	strcpy(serveur.s.serveurname,NAME);										\
	strcat(serveur.s.serveurname,":");										\
	sprintf(serveur.s.serveurname+strlen(NAME)+1,"%ld",PORT);\
}

serveur_t creerServeur(char *nomDuServeur, uint64_t port)
{
/*	uint64_t first_k=0,last_k=0;*/
/*	struct idServeur* next=NULL;*/
/*	uint64_t size_l = last_k - first_k + 1;*/
/*	table_de_hachage_t tab= creerHashTable(size_l);*/
/*	SERVEUR.size = size_l;*/
/*	SERVEUR.firstKey = first_k;*/
/*	SERVEUR.tabl = tab;*/
/*	SERVEUR.next_serv = next;*/
    int yes = 1;
    serveur_t serveur;

    serveur.s.port = port;
    SET_SERVEUR_NAME(nomDuServeur, port);

    serveur.idSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serveur.idSocket < 0) {
	perror("socket()");
	exit(EXIT_FAILURE);
    }

    serveur.serv_addr.sin_family = AF_INET;
    serveur.serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveur.serv_addr.sin_port = htons(port);

    // lose the pesky "Address already in use" error message
    if (setsockopt
	(serveur.idSocket, SOL_SOCKET, SO_REUSEADDR, &yes,
	 sizeof(int)) == -1) {
	perror("setsockopt()");
	exit(-1);
    }

    if (bind
	(serveur.idSocket, (struct sockaddr *) &serveur.serv_addr,
	 sizeof(struct sockaddr_in)) < 0) {
	perror("bind()");
	exit(-1);
    }

    if (listen(serveur.idSocket, LENGTH_LISTEN_QUEUE)) {
	perror("liste()");
	exit(-1);
    }
    return serveur;
}

void *talk_to_client(void *socket)
{
    socket_t sockClient = (socket_t) socket;
    /**
     * Corps de la fonction de routine lors de la création de pthread
     * (apparition d'un client
     */
    printf("test = OK\n");
    /*
     * Fermeture de la socket
     */
    shutdown(sockClient, 2);
    /*
     * On peut utiliser pthread_exit pour renvoyer une valeur (d'erreur ou autre) 
     * => voir pthread_join
     */
    pthread_exit(NULL);
}

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

