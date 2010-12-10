#ifndef COMMUN_H
#define COMMUN_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/errno.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <arpa/inet.h>
#include <netdb.h>

#define MAXCAR 20
#define DEBUG_MESSAGE
//#undef DEBUG_MESSAG


typedef uint32_t socket_t;
typedef char* cle_t;
typedef char* valeur_t;


typedef struct donnee {
	char* cle;
	char* valeur;
}* donnee_t;

typedef struct maillon {
	donnee_t data;
	struct maillon *suiv;
} *liste_t;



static inline donnee_t creerDonnee(cle_t K, valeur_t V)
{
	donnee_t D;
	
	D=malloc(sizeof(struct donnee));
	if(D==NULL){
		perror("malloc()");
		return NULL;
	}
	
	D->cle = (cle_t)malloc(sizeof(cle_t)*strlen(K));
	if(D->cle==NULL){
		free(D);
		perror("malloc()");
		return NULL;
	}
	strcpy(D->cle,K);
	
	D->valeur=(valeur_t)malloc(sizeof(valeur_t)*strlen(V));
	if(D->valeur==NULL){
		free(D->cle);
		free(D);
		perror("malloc()");
		return NULL;
	}
	strcpy(D->valeur,V);
	
	return D;
}

static inline void libererDonnee(donnee_t D){

	free(D->cle);
	free(D->valeur);
	free(D);
}

/** ajoute une donné à la liste pointé par L_ptr 
 ** Permet aussi de creer une liste 		**/
static inline int ajouterDonnee(liste_t * L_ptr, donnee_t D)
{
	
	if(D==NULL){
		return 0;
	}
	liste_t teteDeListe;
	liste_t iterateur;

	//verifie si la cle existe deja
	for (iterateur = *L_ptr; iterateur != NULL; iterateur = iterateur->suiv) {
	//TODO data->cle!=NULL TOUJOuRS ????
		if (strcmp(iterateur->data->cle, D->cle) == 0) {
			free(iterateur->data->valeur);
			iterateur->data->valeur=D->valeur;
			return 1;
		}
	}
	
	/** creation d'un maillon **/
	teteDeListe = (liste_t) malloc(sizeof(struct maillon));
	if (teteDeListe == NULL) {
		perror("malloc()");
		return 0;
	}
	teteDeListe->data = D;
	teteDeListe->suiv = *L_ptr;

	*L_ptr = teteDeListe;
	return 1;
}

static inline donnee_t getKey(liste_t L, cle_t K)
{
	liste_t iterateur;

	for (iterateur = L; iterateur != NULL; iterateur = iterateur->suiv) {

		assert(iterateur->data != NULL);
		if (strcmp(iterateur->data->cle, K) == 0) {
			return iterateur->data;
		}
	}

	return NULL;
}

/** retourne la valeur associé à la clé et remove la clé de la dht **/
static inline valeur_t removeKey(liste_t * L_ptr, cle_t K)
{
	liste_t *iterateur_ptr;
	liste_t *precedent_ptr;
	liste_t *suivant_ptr;
	valeur_t valeur;
	liste_t l;

	iterateur_ptr = NULL;
	precedent_ptr = NULL;
	suivant_ptr = NULL;


	if (*L_ptr == NULL) {
	#ifdef DEBUG_MESSAGE
		printf("removeKey d'une liste vide\n");
	#endif
		return NULL;
	}


	/** on vérifie si le premier maillon contient la clé **/
	if (strcmp((*L_ptr)->data->cle, K) == 0) {
#ifdef DEBUG_MESSAGE
		printf("reloveKey: element enleve en tete de liste\n");
#endif
		valeur = (*L_ptr)->data->valeur;
		l = *L_ptr;
		*L_ptr = l->suiv;
		libererDonnee(l->data);
		free(l);
		return valeur;
	} else {

		precedent_ptr = L_ptr;
		iterateur_ptr = &(*L_ptr)->suiv;

		/** parcour de la liste et reperage du maillon à enlever **/
		while (*iterateur_ptr != NULL) {
			assert((*iterateur_ptr)->data != NULL);
			if (strcmp((*iterateur_ptr)->data->cle, K) == 0) {
				/** le maillon à supprimer est trouve **/
				goto suite;
			}
			precedent_ptr = iterateur_ptr;
			iterateur_ptr = &((*iterateur_ptr)->suiv);
		}
#ifdef DEBUG_MESSAGE
		printf("removeKey: la cle n'est pas dans la liste");
#endif
		return NULL;

 suite:
		valeur = (*iterateur_ptr)->data->valeur;
		suivant_ptr = &((*iterateur_ptr)->suiv);

		libererDonnee((*iterateur_ptr)->data);
		free(*iterateur_ptr);
		(*precedent_ptr)->suiv = *suivant_ptr;

		return valeur;
	}
}

static inline void libererListe(liste_t * L_ptr)
{
	while (*L_ptr != NULL) {
		/** on enlève la tête de liste jusqu'à vider la liste **/
		removeKey(L_ptr, (*L_ptr)->data->cle);
	}
}

/********* methodes de debug *********/
static void afficherDonnee(donnee_t D)
{
	if (D == NULL) {
		printf("dada non init\n");
	} else {
		printf("cle: %s, valeur: %s\n", D->cle ,D->valeur);
	}
}

static void afficherListe(liste_t L)
{
	liste_t iterateur;
	if (L == NULL) {
		printf("la liste à afficher est nul!!\n");
		return;
	}
	for (iterateur = L; iterateur != NULL; iterateur = iterateur->suiv) {
		afficherDonnee(iterateur->data);
	}
}




#endif

