#ifndef MESSAGE_H
#define MESSAGE_H



#include "commun.h"



//============================================================================
//	                             MACROS
//============================================================================

#define OUI 1
#define NON 0

#define T_VALEUR 	   sizeof(valeur_t)
/*
 *Un entier sur 32 s'ecrit au max avec 10 chiffres en base 10
 */
#define T_INT_32           10
/*
 *Un entier sur 64 s'ecrit au max avec 20 chiffres en base 10
 */
#define T_INT_64 	   20
#define T_OCTET            sizeof(char)
#define T_CHAINE(string)   strlen(string)*T_OCTET
#define T_DONNEE(donnee)   T_CHAINE(donnee->cle) + T_CHAINE(donnee->valeur)


#define DEBUG_MESSAGE_H
//#undef  DEBUG_MESSAGE_H
#ifdef DEBUG_MESSAGE_H
#   define print_debug(chaine, args...)   printf(chaine,##args)
#else
#   define print_debug(chaine, args...)
#endif



//============================================================================
//	                                TYPES
//============================================================================

typedef enum  {
	FROM_SERVEUR,
	FROM_CLIENT,
} origine_t;

typedef enum  {
	PUT,
	GET,
	ACK,
	IDENT,
	CONNECT,
	DISCONNECT,
	AUTH_CLIENT,
	AUTH_SERVER,
} requete_t;




//============================================================================
//	                ENVOIS DES TYPES ELEMENTAIRES
//============================================================================


/*
 *envoi d'un entier sur 32 bits.
 *L'entier est converti en chaine de caractere , puis envoyé
 */
static int envoyerUInt_32(uint32_t I, socket_t to)
{

	char *s_I = calloc(T_INT_32,T_OCTET);
	if (s_I == NULL) {
		perror("malloc()");
		return 0;
	}
	sprintf(s_I, "%d", I);

	if (send(to, s_I, T_INT_32, 0) == -1) {
		perror("send()");
		return 0;
	}
	print_debug("envoi de: %d\n", I);
	return 1;
}


/*
 *reception d'un entier sur 32 bits.
 *L'entier est reçu sous forme de chaine de caractere, qui est 
 *retraduite en entier sur 32 bits
 */
static int recevoirUInt_32(uint32_t * I, socket_t from)
{

	char *s_I = calloc(T_INT_32,T_OCTET);
	if (s_I == NULL) {
		perror("malloc()");
		return 0;
	}

	if (recv(from, s_I, T_INT_32, 0) == -1) {
		perror("recv()");
		return 0;
	}
	//TODO EST CE QUIL FAUT FAIRE APPEL A ATOL PLUTOT?
	*I = (uint32_t)atoi(s_I);
	print_debug("recep de: %d\n", *I);
	return 1;
}


/*
 *envoi d'un entier sur 64 bits.
 *L'entier est converti en chaine de caractere , puis envoyé
 */
static envoyerUInt_64(uint64_t I, socket_t to)
{

	char *s_I = calloc(T_INT_64,T_OCTET);
	if (s_I == NULL) {
		perror("malloc()");
		return 0;
	}
	sprintf(s_I, "%ld", I);

	if (send(to, s_I, T_INT_64, 0) == -1) {
		perror("send()");
		return 0;
	}
	print_debug("envoi de: %ld\n", I);
	return 1;
}



/*
 *reception d'un entier sur 64 bits.
 *L'entier est reçu sous forme de chaine de caractere, qui est 
 *retraduite en entier sur 64 bits
 */
static recevoirUInt_64(uint64_t * I, socket_t from)
{
	char *s_I = calloc(T_INT_64,T_OCTET);
	if (s_I == NULL) {
		perror("malloc()");
		return 0;
	}
	if (recv(from, s_I, T_INT_64, 0) == -1) {
		perror("recv()");
		return 0;
	}
	//TODO UN PEU OPTIMISTE ON SUPPOSE QUE ATOLL RECOI BIEN UN NETIER EN CHAINE
	*I = atoll(s_I);
	print_debug("recep de: %ld\n", *I);
	return 1;
}


/*
 *envoi d'un octet
 */
static int envoyerOctet(char O, socket_t to)
{
	if (send(to, &O, T_OCTET, 0) == -1) {
		perror("send");
		return 0;
	}
	print_debug("envoi de: %c\n", O);
	return 1;
}

/*
 *reception d'un octet
 */
static int recevoirOctet(char *O, socket_t from)
{
	if (recv(from, O, T_OCTET, 0) == -1) {
		perror("recv()");
		return 0;
	}
	print_debug("recep de: %c\n", *O);
	return 1;
}


/*
 * envoi d'une chaine de caractere.D'abord on envoi la taille puis la chaine elle meme
 */
static int envoyerChaine(char *chaine, socket_t to)
{
	uint32_t taille_chaine = T_CHAINE(chaine);
	print_debug("\t:");
	if (!envoyerUInt_32(taille_chaine, to)) {
		return 0;
	}
	print_debug("\t:");
	if (send(to, chaine, taille_chaine, 0) == -1) {
		perror("send()");
		return 0;
	}
	print_debug("envoi de: %s\n", chaine);
	return 1;
}


/*
 *reception d'une chaine de caractere
 */
static int recevoirChaine(char **chaine, socket_t from)
{

	uint32_t taille_chaine;
	print_debug("\t:");
	if (!recevoirUInt_32(&taille_chaine, from)) {
		return 0;
	}
	print_debug("\t:");
	*chaine = (char *)calloc(taille_chaine,T_OCTET);
	if (*chaine == NULL) {
		perror("malloc");
		return 0;
	}
	if (recv(from, *chaine, taille_chaine, 0) == -1) {
		perror("recv()");
		return 0;
	}
	print_debug("recep de: %s\n", *chaine);
	return 1;
}
	
/* envoyer une valeur ou une cle revient à envoyer une chaine de caractere*/
#define envoyerValeur(valeur,to)  envoyerChaine(valeur,to)
#define envoyerCle(cle,to)        envoyerChaine(cle,to)

#define recevoirValeur(valeur,to)  recevoirChaine(valeur,to)
#define recevoirCle(cle,to)        recevoirChaine(cle,to)


/*
 *envoi une donné
 */
static int envoyerDonnee(donnee_t D, socket_t to)
{
	return (envoyerChaine(D->cle, to) & envoyerChaine(D->valeur, to));
}


/*
 *reception d'une donné
 */
static int recevoirDonnee(donnee_t * D, socket_t from)
{
	cle_t cle;
	valeur_t val;

	recevoirCle(&cle, from);
	recevoirValeur(&val, from);

	*D = creerDonnee(cle, val);
	return 1;
}


/*
 *envoi le type de message
 */
static int envoyerTypeMessage(requete_t I, socket_t to)
{
	return envoyerUInt_32(I, to);
}

/*
 *reception du type de message
 */
static int recevoirTypeMessage(requete_t * I, socket_t from)
{
	return recevoirUInt_32(I, from);
}

/*
 *envoi de l'origine du message
 */
static int envoyerOrigine(origine_t I, socket_t to)
{
	return envoyerUInt_32(I, to);
}

/*
 *reception de l'origine du message
 */
static int recevoirOrigine(origine_t * I, socket_t from)
{
	return recevoirUInt_32(I, from);
}

/*
 * envoi de l'identité du client
 */
static int envoyerIdent(struct sockaddr_in ident, socket_t to)
{
	envoyerChaine(inet_ntoa(ident.sin_addr), to);
	envoyerUInt_32(ident.sin_port, to);
//    envoyerUInt_32(ident.sin_family,to);
	return 1;
}

/*
 *recption de l'identité du client ou serveur
 */
static int recevoirIdent(struct sockaddr_in *ident, socket_t from)
{
	char *ip;
	uint32_t port;
	
	recevoirChaine(&ip, from);
	recevoirUInt_32(&port, from);

	ident->sin_addr.s_addr = inet_addr(ip);
	ident->sin_port = port;
	return 1;
}

/*
 *envoi un hash.Ca peut etre l'envoi de l'identifiant d'un serveur
 */
static int envoyerHash(uint64_t h, socket_t to)
{
	return envoyerUInt_64(h, to);
}

/*
 *reception d'un resultat de la fonction de hachage
 */
static int recevoirHash(uint64_t * h, socket_t from)
{
	return recevoirUInt_64(h, from);
}


#endif

