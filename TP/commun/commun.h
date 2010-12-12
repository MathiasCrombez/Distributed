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





//==============================================================================
//				TYPES COMMUNS
//=============================================================================

typedef uint32_t socket_t;
typedef char* cle_t;
typedef char* valeur_t;


typedef struct donnee {
	char* cle;
	char* valeur;
}* donnee_t;


/*
 * identifie une connexion avec un serveur ou un client
 * contient l'ip , le port , le nom du client et un socket pour
 * communiquer avec lui
 */ 
typedef struct idConnexion {

	struct sockaddr_in identifiant;
	char *name;
	uint64_t h;
	uint64_t taille_hashtab;
} idConnexion_t;
//==============================================================================
//			       MACROS
//==============================================================================


#define MAXCAR 20
#define DEBUG_MESSAGE


//==============================================================================
//			       FONCTIONS
//==============================================================================


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



/********* methodes de debug *********/
static void afficherDonnee(donnee_t D)
{
	if (D == NULL) {
		printf("dada non init\n");
	} else {
		printf("cle: %s, valeur: %s\n", D->cle ,D->valeur);
	}
}


/********* methodes de debug *********/
static void afficherIdentConnexion(struct idConnexion *ident)
{
	printf("identifiant connexion:\n");
	
	if(ident==NULL){
		printf("identifiant non init\n");
	} else {
		printf("\tNom       : %s\n", ident->name);
		printf("\tAdresse Ip: %s\n", inet_ntoa(ident->identifiant.sin_addr));
		printf("\tPort conne: %d\n", ntohs(ident->identifiant.sin_port));
		printf("\tProtocole : AF_INET=%d recu:%d\n",AF_INET ,ident->identifiant.sin_family);
	}
}
#endif

