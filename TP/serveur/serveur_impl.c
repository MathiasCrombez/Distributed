#include "serveur_impl.h"

#define SET_SERVEUR_NAME(NAME,PORT){								\
	strcpy(SERVEUR.name,NAME);										\
	strcat(SERVEUR.name,":");										\
	sprintf(SERVEUR.name+strlen(NAME)+1,"%ld",PORT);                \
}


serveur_t creerServeur(char *nomDuServeur, uint64_t port)
{
    int yes = 1;
    
   
    
    SET_SERVEUR_NAME(nomDuServeur, port);
    SERVEUR.idSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (SERVEUR.idSocket < 0) {
	    perror("socket()");
	    exit(EXIT_FAILURE);
    }
    #define SERVEURNAME "194.254.210.43"
    SERVEUR.serv_addr.sin_family = AF_INET;
    //SERVEUR.serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   SERVEUR.serv_addr.sin_addr.s_addr = inet_addr(SERVEURNAME);
    SERVEUR.serv_addr.sin_port = htons(port);
    SERVEUR.nextServeur.identifiant =  SERVEUR.serv_addr;
   
    SERVEUR.firstKey=0;
    SERVEUR.nextKey = 0;
    
    // lose the pesky "Address already in use" error message
    if (setsockopt(SERVEUR.idSocket, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
	    perror("setsockopt()");
	    exit(-1);
    }

    if (bind(SERVEUR.idSocket, (struct sockaddr *) &SERVEUR.serv_addr,sizeof( SERVEUR.serv_addr)) < 0) {
	    perror("bind()");
	    exit(-1);
    }

    if (listen(SERVEUR.idSocket, LENGTH_LISTEN_QUEUE)) {
	    perror("liste()");
	    exit(-1);
    }
    return SERVEUR;
}


void talk_to_client(socket_t socket)
{
    //ce socket va nous permettre de communiquer avec le client
    socket_t sockClient = (socket_t) socket;

    /**
     * Corps de la fonction de routine lors de la création de pthread
     * (apparition d'un client
     */
    requete_t type_requete;
    recevoirTypeMessage(&type_requete,sockClient);
    
    switch(type_requete){
    
    case  PUT:
        break;
        
    case  GET:
        break;
        
    case  ACK:
        break;
    
    case  IDENT:
        break;
    
     case  CONNECT:
        printf("un serveur veut participer à la dht\n");
        
        //#### debug ####//
        printf("mon prochain serveur est:\n");
        printf("SERVEUR.name %s\n",SERVEUR.name );
        
		afficherIdentConnexion(SERVEUR.nextServeur.identifiant);
		//#### debug ####//
		
        envoyerIdent(SERVEUR.nextServeur.identifiant, sockClient);
        envoyerHash(SERVEUR.nextKey,sockClient);
        recevoirIdent(&(SERVEUR.nextServeur.identifiant), sockClient);
        recevoirHash(&(SERVEUR.nextKey),sockClient);
        
        //#### debug ####//
        printf("mon prochain serveur est:\n");
		afficherIdentConnexion(SERVEUR.nextServeur.identifiant);
        //#### debug ####//
        break;
    
    default  :
        break;

    }
    
}


socket_t connect2server( char* to_serveur,uint64_t port){
    
	struct hostent *hostinfo = NULL;
	socket_t idSocket;
	struct sockaddr_in ddr;
	
	hostinfo = gethostbyname(to_serveur);
	if (hostinfo == NULL) {
		printf("erreur gethostbyname():le serveur %s est inconnu\n", to_serveur);
		return 0;
	}

    ddr.sin_family = AF_INET;
	ddr.sin_port   = htons(port);
	ddr.sin_addr   = *(struct in_addr *) hostinfo->h_addr;
	idSocket = socket(AF_INET, SOCK_STREAM, 0);

	
    if (idSocket < 0) {
	    perror("socket()");
	    return 0;
    }
    
	if (connect(idSocket, (struct sockaddr *)&ddr,sizeof(struct sockaddr_in)) ==-1) {
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





