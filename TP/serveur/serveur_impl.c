#include "serveur_impl.h"
#include <stdio.h>
#include <string.h>


serveur_t SERVEUR;



serveur_t creerServeur(char serveurname[MAXCAR],uint64_t first_k,
							uint64_t last_k, struct idServeur* next)
{
	uint64_t size_l = last_k - first_k + 1;
	hash_t *tab[size_l];
	int i;
	for (i = 0; i < size_l; i++) {
		tab[i] = NULL;
	}
	strncpy(SERVEUR.s.serveurname,serveurname,MAXCAR);
	
#ifdef DEBUG_MESSAGE
	printf("serveurname %s \n",SERVEUR.s.serveurname);
#endif

	SERVEUR.s.port = PORT;
	SERVEUR.size = size_l;
	SERVEUR.firstKey = first_k;
	SERVEUR.tabl = tab;
	SERVEUR.next_serv = next;
	SERVEUR.idSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(SERVEUR.idSocket<0){
        perror("socket()");
	    exit(EXIT_FAILURE);
    }
        
    SERVEUR.serv_addr.sin_family = AF_INET;
    SERVEUR.serv_addr.sin_addr.s_addr = INADDR_ANY;
    SERVEUR.serv_addr.sin_port = htons(PORT);
    
    if (bind(SERVEUR.idSocket,(struct sockaddr *)&SERVEUR.serv_addr,sizeof(struct sockaddr_in)) < 0) {
	    perror("bind()");
	    exit(-1);
    }
    
    if(listen(SERVEUR.idSocket,LENGTH_LISTEN_QUEUE)) {
        perror("liste()");
        exit(-1);
    }
	return SERVEUR;
}

void *talk_to_client(void *donne)
{

	int idSocket = (int)donne;
	/*    message_t ms;

	   bzero(ms.data,MESSAGE_SIZE);
	   strcpy(ms.data,"Tp DHT, mathias ramzi yongzhi");
	   strcat(ms.data,"\n");
	   send(idSocket,ms.data,MESSAGE_SIZE,0);
	   bzero(ms.data,MESSAGE_SIZE);     

	   int longueur = recv(idSocket,ms.data,MESSAGE_SIZE,0);
	   if (longueur < 0) {

	   printf("Serveur Recieve Data Failed!\n");
	   exit(1);
	   }
	   printf("\nSocket Num: %d \t %s",idSocket, ms.data);
	 */
	shutdown(idSocket, 2);
	pthread_exit(NULL);
}

int put_h(serveur_t s, uint64_t cle, char *valeur, uint64_t taille)
{
	char str[taille];
	hash_t *curr, *prec, *newCell;
	int i;
	uint64_t key_l = cle - s.firstKey;
	newCell =
	    malloc(sizeof(char) * taille + sizeof(taille) + sizeof(hash_t *));
	for (i = 0; i < taille; i++) {
		str[i] = valeur[i];
	}
	prec = s.tabl[key_l];
	curr = prec;
	while (curr != NULL) {
		prec = curr;
		curr = curr->suiv;
	}
	newCell->size = taille;
	newCell->value = str;
	newCell->suiv = NULL;
	return 0;
}

uint64_t get_h(serveur_t s, uint64_t cle, char *valeur)
{
	uint64_t key_l = cle - s.firstKey;
	hash_t *curr = s.tabl[key_l];
	while (curr != NULL && strcmp(curr->value, valeur)) {
		curr = curr->suiv;
	}
	if (curr == NULL) {
		valeur = NULL;
		return 0;
	} else {
		valeur = curr->value;
		return curr->size;
	}
}

int remove_h(serveur_t s, uint64_t cle, char *valeur, uint64_t taille)
{
	int key_l = cle - s.firstKey;
	hash_t *curr, *prec;
	curr = s.tabl[key_l];
	if (curr != NULL && curr->size == taille
	    && !strcmp(curr->value, valeur)) {
		s.tabl[key_l] = curr->suiv;
		free(curr);
		return 0;
	} else {
		while (curr != NULL && curr->size != taille
		       && !strcmp(curr->value, valeur)) {
			prec = curr;
			curr = curr->suiv;
		}
		if (curr == NULL) {
			return -1;
		} else {
			prec->suiv = curr->suiv;
			free(curr);
			return 0;
		}
	}
}

