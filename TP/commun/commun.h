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

#define DEBUG_MESSAGE
 

typedef struct donnee{
	char cle[MAXCAR];
	char valeur[MAXCAR];
}* donnee_t;


typedef struct maillon{
	donnee_t data;
	struct maillon* suiv;
}* liste_t ;



static inline donnee_t creerDonnee(char* K, char* V){

	donnee_t D;
	D=(donnee_t)malloc(sizeof(struct donnee));
	
	if(D==NULL){
		perror("malloc()");
		exit(-1);
	}
	
	strncpy(D->cle,K,MAXCAR);
	strncpy(D->valeur,V,MAXCAR);

	return D;
}


static inline donnee_t getKey(liste_t L, char* str){

	liste_t iterateur;
	
	for(iterateur=L;iterateur != NULL ;iterateur = iterateur -> suiv ){
		
		if(iterateur->data !=NULL && strcmp(iterateur->data->cle,str)==0 ){
			return iterateur->data;
		}
	}
	
	return NULL;

}


static inline void ajouterDonnee(liste_t* L_ptr , donnee_t D){
 
	
	
	/** creation d'un maillon **/
	
	liste_t teteDeListe;
	
	teteDeListe= (liste_t)malloc(sizeof(struct maillon));
	if(teteDeListe==NULL){
		perror("malloc()");
		exit(-1);
	}
	teteDeListe->data= D;
	teteDeListe->suiv = *L_ptr;
	
	*L_ptr=teteDeListe;
}


static inline void afficherListe(liste_t L){

	liste_t iterateur;
	for(iterateur=L;iterateur != NULL ;iterateur = iterateur -> suiv ){
		if(iterateur->data==NULL){
			printf("dada non init");
			return;
		}else{
			printf("cle: %s, valeur: %s \n",iterateur->data->cle,iterateur->data->valeur);
		}
	}
}


#endif
