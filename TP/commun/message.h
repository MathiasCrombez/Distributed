#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/socket.h>
#include "commun.h"

#define T_VALEUR 		sizeof(valeur_t)
#define T_INT_32 		32
#define T_OCTET 		sizeof(char)
#define T_CHAINE(string) strlen(string)*T_OCTET
#define T_DONNEE(donnee)   T_CHAINE(donnee->cle) + T_CHAINE(donnee->valeur)

#define SET_MESSAGE(M,type_r,len)                               \
    message_t M;                                                       \
                                                \
    M.type_requete = type_r;                                           \
    M.taille = len;                                                    \

//enum ___type_message {
//      REQUETE,
//      REPONSE,
//};

typedef enum _type_requete {
	INT_32,
	CHAINE, 
	AUTRE,
} requete_t;



typedef struct message {

	//enum ___type_message type_mess;
	enum _type_requete type_requete;
	uint32_t taille;
	void* data;

} message_t;



static inline ssize_t recevoirMessage(message_t* message_ptr, socket_t expediteur)
{
	ssize_t n_octet;
	uint32_t taille;
	requete_t r;
	
	recv(expediteur, &r, sizeof(enum _type_requete),0);
	recv(expediteur, &taille, sizeof(uint32_t), 0);

    message_ptr->data = malloc(taille);
	if (n_octet = (recv(expediteur,message_ptr->data, taille, 0)) == -1) {
		perror("send()");
		exit(-1);
	}
	
//	message_ptr->type_requete = r;
//    message_ptr->taille= taille;
//    message_ptr->data=m;
    
#ifdef DEBUG_MESSAGE
	printf("reception d'un message du serveur/client ...\n");
#endif
	return n_octet;
}


static inline uint32_t recevoirINT_32(socket_t to){

    ssize_t n_octet;
    uint32_t I;
    recv(to,&I,sizeof(uint32_t),0);
    return I;
}


static ssize_t envoyerINT_32(uint32_t I, socket_t to)
{
	ssize_t n_octet;
	send(to, &I, sizeof(uint32_t), 0);
#ifdef DEBUG_MESSAGE
	printf("envoie d'un message au serveur/client ...\n");
#endif
	return n_octet;
}





static ssize_t envoyerChaine(char *chaine, socket_t to)
{
    uint32_t size=T_CHAINE(chaine);
	ssize_t n_octet;
	
	envoyerINT_32(size,to);
	
	if (n_octet = (send(to, chaine, size, 0)) == -1) {
		perror("send()");
		exit(-1);
	};
#ifdef DEBUG_MESSAGE
	printf("envoie d'un message au serveur/client ...\n");
#endif
	return n_octet;
}


static char* recevoirChaine( socket_t to){

    ssize_t n_octet;
    uint32_t taille;
    char* chaine;
   
    
    taille = recevoirINT_32(to);
     printf("taille de chaiine: %d\n",taille);
     
    if((chaine) = (char*)malloc(taille)==NULL){
        perror("malloc()");
        exit(-1);
    }
    
    
    if(n_octet=(recv(to,chaine,taille,MSG_WAITALL))==-1){
        perror("recv()");
        exit(-1);
    }
    printf("chaine reçue: %s\n",chaine);
    return chaine;
}

/*
 * envoyer une valeur ou une cle revient à envoyer une chaine de
 * caractere
 */
#define envoyerValeur(valeur,to)  envoyerChaine(valeur,to)
#define envoyerCle(cle,to)        envoyerChaine(cle,to)


//static ssize_t envoyerDonnee(donnee_t D, socket_t to)
//{
//	return envoyerChaine(D->cle, to) + envoyerChaine(D->valeur, to);
//}

void string_message(char *chaine);
void put_message(donnee_t D, socket_t to);
valeur_t get_message(cle_t, socket_t to);
#endif

