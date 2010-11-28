#ifndef COMMUN_H
#define COMMUN_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef uint32_t socket_t;

#define PORT 4242
#define MAXCAR 20

//#define DEBUG_MESSAGE
#undef DEBUG_MESSAGE

typedef struct donnee {
	char cle[MAXCAR];
	char valeur[MAXCAR];
} *donnee_t;

typedef struct maillon {
	donnee_t data;
	struct maillon *suiv;
} *liste_t;

static inline donnee_t creerDonnee(char *K, char *V)
{

	donnee_t D;
	D = (donnee_t) malloc(sizeof(struct donnee));

	if (D == NULL) {
		perror("malloc()");
		exit(-1);
	}

	strncpy(D->cle, K, MAXCAR);
	strncpy(D->valeur, V, MAXCAR);

	return D;
}

static inline donnee_t getKey(liste_t L, char *str)
{

	liste_t iterateur;

	for (iterateur = L; iterateur != NULL; iterateur = iterateur->suiv) {

		assert(iterateur->data != NULL);
		if (strcmp(iterateur->data->cle, str) == 0) {
			return iterateur->data;
		}
	}

	return NULL;

}

/** retourne la valeur associé à la clé et remove la clé de la dht **/
static inline char *removeKey(liste_t * L_ptr, char *str)
{

	liste_t *iterateur_ptr;
	liste_t *precedent_ptr;
	liste_t *suivant_ptr;

	iterateur_ptr = L_ptr;
	precedent_ptr = L_ptr;
	suivant_ptr = NULL;

#ifdef DEBUG_MESSAGE
	if (*L_ptr == NULL) {
		printf("removeKey d'une liste vide\n");
		return NULL;
	}
#endif

	char *valeur;
	/** parcour de la liste et reperage du maillon à enlever **/
	while (*iterateur_ptr != NULL) {
		assert((*iterateur_ptr)->data != NULL);
		if (strcmp((*iterateur_ptr)->data->cle, str) == 0) {
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

	free((*iterateur_ptr)->data);
	free(*iterateur_ptr);
	(*precedent_ptr)->suiv = *suivant_ptr;

	return valeur;
}

static inline void ajouterDonnee(liste_t * L_ptr, donnee_t D)
{

	/** creation d'un maillon **/
	liste_t teteDeListe;

	teteDeListe = (liste_t) malloc(sizeof(struct maillon));
	if (teteDeListe == NULL) {
		perror("malloc()");
		exit(-1);
	}
	teteDeListe->data = D;
	teteDeListe->suiv = *L_ptr;

	*L_ptr = teteDeListe;
}

static void afficherDonnee(donnee_t D)
{

	if (D == NULL) {
		printf("dada non init\n");
	} else {
		printf("cle: %s, valeur: %s \n", D->cle, D->valeur);
	}
}

static void afficherListe(liste_t L)
{

	liste_t iterateur;
	if (L == NULL) {
		printf("la liste à afficher est nul!!\n");
		return ;
	}
	for (iterateur = L; iterateur != NULL; iterateur = iterateur->suiv) {
		afficherDonnee(iterateur->data);
	}
}

#endif

