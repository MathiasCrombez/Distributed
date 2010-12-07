#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/socket.h>
#include "commun.h"
#include <rpc/xdr.h>

#define T_VALEUR 		sizeof(valeur_t)
#define T_INT_32 		32
#define T_OCTET 		sizeof(char)
#define T_CHAINE(string) strlen(string)*T_OCTET
#define T_DONNEE(donnee)   T_CHAINE(donnee->cle) + T_CHAINE(donnee->valeur)

#define SET_MESSAGE(M,type_r,len)                                      \
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
	void *data;

} message_t;

//static inline ssize_t recevoirMessage(message_t * message_ptr,
//				      socket_t expediteur)
//{
//	ssize_t n_octet;
//	uint32_t taille;
//	requete_t r;

//	recv(expediteur, &r, sizeof(enum _type_requete), 0);
//	recv(expediteur, &taille, sizeof(uint32_t), 0);

//	message_ptr->data = malloc(taille);
//	if (n_octet = (recv(expediteur, message_ptr->data, taille, 0)) == -1) {
//		perror("send()");
//		exit(-1);
//	}
////      message_ptr->type_requete = r;
////    message_ptr->taille= taille;
////    message_ptr->data=m;

//#ifdef DEBUG_MESSAGE
//	printf("reception d'un message du serveur/client ...\n");
//#endif

//	return n_octet;
//}

static inline int recevoirINT_32(uint32_t * int_ptr, socket_t from)
{
	XDR xdrs;
	FILE *fd = fdopen(from, "r");
	xdrstdio_create(&xdrs, fd, XDR_DECODE);

	if (xdr_int(&xdrs, int_ptr) == 0) {
		printf("reception d'entier perdu ...\n");
		return 0;
	}
//#ifdef DEBUG_MESSAGE
	printf("reception d'un int: %d \n", *int_ptr);
//#endif
	fflush(fd);
	return 1;

}

static int envoyerINT_32(uint32_t I, socket_t to)
{

	XDR xdrs;

	FILE *fd = fdopen(to, "w");
	xdrstdio_create(&xdrs, fd, XDR_ENCODE);

	if (xdr_int(&xdrs, &I) == 0) {
		printf("envoi d'entier perdu ...\n");
		return 0;
	}
//#ifdef DEBUG_MESSAGE
	printf("envoi d'un int: %d ...\n", I);
//#endif
//    fflush(fd);
	return 1;
}

static ssize_t envoyerChaine(char *chaine, socket_t to)
{
	uint32_t size = T_CHAINE(chaine);
	XDR xdrs;
	FILE *fd = fdopen(to, "w");
	xdrstdio_create(&xdrs, fd, XDR_ENCODE);

	
	if (xdr_int(&xdrs, &size) && xdr_string(&xdrs, &chaine, size) == 0) {
	    printf("envoi d'un chaine perdu");
		return 0;
	}
#ifdef DEBUG_MESSAGE
	printf("envoi d'un chaine :%s \n",chaine);
#endif
//    fflush(fd);
	return 1;
}

static int recevoirChaine(char **chaine, socket_t from)
{
	uint32_t size;
	XDR xdrs;
	FILE *fd = fdopen(from, "r");
	xdrstdio_create(&xdrs, fd, XDR_DECODE);

	
	*chaine = (char *)malloc(size);

	if (xdr_int(&xdrs, &size) && xdr_string(&xdrs, chaine, size) == 0) {
		printf("recep chaine perd ...\n");
		return 0;
	}
//#ifdef DEBUG_MESSAGE
	printf("recep d'un message : %s  ...\n", *chaine);
//#endif
	fflush(fd);
	return 1;
}

/*
 * envoyer une valeur ou une cle revient à envoyer une chaine de
 * caractere
 */
#define envoyerValeur(valeur,to)  envoyerChaine(valeur,to)
#define envoyerCle(cle,to)        envoyerChaine(cle,to)

//static ssize_t envoyerDonnee(donnee_t D, socket_t to)
//{
//      return envoyerChaine(D->cle, to) + envoyerChaine(D->valeur, to);
//}

void string_message(char *chaine);
void put_message(donnee_t D, socket_t to);
valeur_t get_message(cle_t, socket_t to);
#endif

