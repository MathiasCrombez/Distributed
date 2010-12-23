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
	cle_t cle;
	valeur_t valeur;
}* donnee_t;


/*
 * identifie une connexion avec un serveur ou un client
 * contient l'ip , le port , le nom du client et un socket pour
 * communiquer avec lui
 */ 
typedef struct idConnexion {

	struct sockaddr_in identifiant;
	char name[20];
	uint64_t h;
	uint32_t taille_hashtab;
} idConnexion_t;
//==============================================================================
//			       MACROS
//==============================================================================


#define MAXCAR 20
#define DEBUG_MESSAGE


//==============================================================================
//			       FONCTIONS DONNE
//==============================================================================


static donnee_t creerDonnee(cle_t K, valeur_t V)
{
	donnee_t D;
	
	D=malloc(sizeof(struct donnee));
	if(D==NULL){
		perror("malloc()");
		return NULL;
	}
	
	D->cle = (cle_t)malloc(sizeof(char)*strlen(K));
	if(D->cle==NULL){
		free(D);
		perror("malloc()");
		return NULL;
	}
	strcpy(D->cle,K);
	
	D->valeur=(valeur_t)malloc(sizeof(char)*strlen(V));
	if(D->valeur==NULL){
		free(D->cle);
		free(D);
		perror("malloc()");
		return NULL;
	}
	strcpy(D->valeur,V);
	
	return D;
}

static void libererDonnee(donnee_t D){

        if(D==NULL)
                return;
                
	free(D->cle);
	free(D->valeur);
	free(D);
}



/********* methodes de debug *********/
static void afficherDonnee(donnee_t D)
{
	if (D == NULL) {
		printf("afficherDonnee:Donnee nulle\n");
	} else {
		printf("afficherDonnee:cle( %s ), valeur( %s )\n", D->cle ,D->valeur);
	}
}



//==============================================================================
//			       FONCTIONS IDCONNEXION
//==============================================================================


static idConnexion_t setIdConnexion( char* name,
                                       struct sockaddr_in info,
                                       uint64_t h,
                                       uint32_t size
                                     )
{

        idConnexion_t id;
        
        
	strncpy(id.name,name,19);
	
	id.identifiant= info;
	id.h = h;
	id.taille_hashtab = size;

        return id;
}



/********* methodes de debug *********/
static void afficherIdentConnexion(idConnexion_t ident)
{
	printf("afficherIdentConnexion:\n");
	

	printf("\tNom       : %s\n", ident.name);
	printf("\tAdresse Ip: %s\n", inet_ntoa(ident.identifiant.sin_addr));
	printf("\tPort conne: %d\n", ntohs(ident.identifiant.sin_port));
	printf("\tProtocole : AF_INET=%d recu:%d\n",AF_INET ,ident.identifiant.sin_family);
#ifdef SERVEUR_IMPL_H
	printf("\th         : %llu\n",ident.h);
	printf("\tsize hasht: %u\n",ident.taille_hashtab);
#endif
}



#endif

