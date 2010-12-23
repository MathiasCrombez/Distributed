#ifndef MESSAGE_COMMUN_H
#define MESSAGE_COMMUN_H



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


//#define DEBUG_MESSAGE_H
#undef  DEBUG_MESSAGE_H

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
    REMOVEKEY,
    ACK,
    IDENT,
    WHOIS_NEXT_SERVER,
    CONNECT,
    DISCONNECT,
    RECEIVE_DHT,
    AUTH_SERVER,
    QUIT
} requete_t;




//============================================================================
//	                ENVOIS DES TYPES ELEMENTAIRES
//============================================================================


/*
 *envoi d'un octet de synchro
 */
static int envoyerSynchro(socket_t to)
{
    /* char O = 0; */
    /* if (send(to, &O, T_OCTET, 0) == -1) { */
    /*     perror("envoyerSynchro:send()"); */
    /*     exit(-1); */
    /* } */
    /* print_debug("envoyerSynchro:%d\n", (int)O); */
    return 1;
}

/*
 *reception d'un octet de synchro
 */
static int recevoirSynchro(socket_t from)
{
    /* char O; */
    /* if ( (recv(from, &O, T_OCTET, 0) == -1) && O != 0) { */
    /*     perror("recevoirSynchro:recv()"); */
    /*     exit(-1); */
    /* } */
    /* print_debug("recevoirSynchro:%d\n", (int)*O); */
    return 1;
}

/*
 *envoi d'un octet
 */
static int envoyerOctet(char O, socket_t to)
{
    recevoirSynchro(to);
    if (send(to, &O, T_OCTET, 0) == -1) {
        perror("envoyerOctet:send()");
        exit(-1);
    }
    print_debug("envoyerOctet:%d\n", (int)O);
    return 1;
}

/*
 *reception d'un octet
 */
static int recevoirOctet(char *O, socket_t from)
{
    envoyerSynchro(from);
    if (recv(from, O, T_OCTET, 0) == -1) {
        perror("recevoirOctet:recv()");
        exit(-1);
    }
    print_debug("recevoirOctet:%d\n", (int)*O);
    return 1;
}

/*
 *envoi d'un entier sur 32 bits.

 *L'entier est converti en chaine de caractere , puis envoyé
 */
static int envoyerUInt_32(uint32_t I, socket_t to)
{

    char *s_I;
    char ack;
    if ( ( s_I = calloc(T_INT_32,T_OCTET) ) == NULL) {
        perror("envoyerUInt_32:calloc()");
        exit(-1);
    }
    sprintf(s_I, "%u", I);
    recevoirSynchro(to);    
    if (send(to, s_I, T_INT_32, 0) == -1) {
        perror("envoyerUInt_32:send()");
        exit(-1);
    }
    print_debug("envoyerUInt_32:%u\n", I);
    free(s_I);
    return 1;
}


/*
 *reception d'un entier sur 32 bits.
 *L'entier est reçu sous forme de chaine de caractere, qui est
 *retraduite en entier sur 32 bits
 */
static int recevoirUInt_32(uint32_t * I, socket_t from)
{

    char *s_I;
    if ( ( s_I = calloc(T_INT_32,T_OCTET) ) == NULL) {
        perror("recevoirUInt_32:calloc()");
        exit(-1);
    }
    envoyerSynchro(from);
    if (recv(from, s_I, T_INT_32, 0) == -1) {
        perror("recevoirUInt_32:recv()");
        exit(-1);
    }
    *I = (uint32_t)atol(s_I);
    free(s_I);
    print_debug("recevoirUInt_32:%u\n", *I);
    return 1;
}


/*
 *envoi d'un entier sur 64 bits.
 *L'entier est converti en chaine de caractere , puis envoyé
 */
static int envoyerUInt_64(uint64_t I, socket_t to)
{

    char *s_I;
    char ack;
    if ( ( s_I = calloc(T_INT_64,T_OCTET) ) == NULL) {
        perror("envoyerUInt_64:calloc()");
        exit(-1);
    }
	
#if __WORDSIZE == 64
    sprintf(s_I, "%lu", I);
#else
    sprintf(s_I, "%llu", I);
#endif
    recevoirSynchro(to);
    if (send(to, s_I, T_INT_64, 0) == -1) {
        perror("envoyerUInt_64:send()");
        exit(-1);
    }
#if __WORDSIZE == 64
    print_debug("envoyerUInt_64:%lu\n", I);
#else
    print_debug("envoyerUInt_64:%llu\n", I);
#endif
    free(s_I);
    return 1;
}
/*
 *reception d'un entier sur 64 bits.
 *L'entier est reçu sous forme de chaine de caractere, qui est
 *retraduite en entier sur 64 bits
 */
static int recevoirUInt_64(uint64_t * I, socket_t from)
{
    char *s_I;
    if ( ( s_I = calloc(T_INT_64,T_OCTET) ) == NULL) {
        perror("recevoirUInt_64:calloc()");
        exit(-1);
    }
    envoyerSynchro(from);
    if (recv(from, s_I, T_INT_64, 0) == -1) {
        perror("recevoirUInt_64:recv()");
        exit(-1);
    }
    //TODO UN PEU OPTIMISTE ON SUPPOSE QUE ATOLL RECOI BIEN UN NETIER EN CHAINE
	
    *I = atoll(s_I);
#if __WORDSIZE == 64
    print_debug("recevoirUInt_64:%lu\n", *I);
#else
    print_debug("recevoirUInt_64:%llu\n", *I);
#endif
    free(s_I);

    return 1;
}


/*
 * envoi d'une chaine de caractere.D'abord on envoi la taille puis la chaine elle meme
 */
static int envoyerChaine(char *chaine, socket_t to)
{
    char ack;
    uint32_t taille_chaine;
    taille_chaine = T_CHAINE(chaine);
    print_debug("\t:");
    if (!envoyerUInt_32(taille_chaine, to)) {
        exit(-1);
    }
    print_debug("\t:");
    recevoirSynchro(to);
    if (send(to, chaine, taille_chaine, 0) == -1) {
        perror("envoyerChaine:send()");
        exit(-1);
    }
    print_debug("envoyerChaine:%s\n", chaine);
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
        exit(-1);
    }
    print_debug("\t:");

    if ( ( *chaine = (char *)calloc(taille_chaine,T_OCTET) ) == NULL) {
        perror("calloc");
        exit(-1);
    }
    envoyerSynchro(from);
    if (recv(from, *chaine, taille_chaine, 0) == -1) {
        perror("recevoirChaine:recv()");
        exit(-1);
    }
       
    print_debug("recevoirChaine:%s\n",*chaine);

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
    print_debug("envoyerDonnee:Debut\n");
    envoyerChaine(D->cle, to);
    envoyerChaine(D->valeur, to);
    print_debug("envoyerDonnee:Fin\n");
    return 1;
}


/*
 *reception d'une donné
 */
static int recevoirDonnee(donnee_t * D, socket_t from)
{
    cle_t K;
    valeur_t V;
	
    print_debug("recevoirDonnee:Debut\n");
    recevoirCle(&K, from);
    recevoirValeur(&V, from);
    *D = creerDonnee(K, V);
    free(K);
    free(V);
    print_debug("recevoirDonnee:Fin\n");
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



#define envoyerHash( h, to) envoyerUInt_64(h, to)
#define recevoirHash(h,from) recevoirUInt_64(h, from)

/*
 * envoi de l'identité du client
 */
static int envoyerIdent(idConnexion_t ident, socket_t to)
{

    print_debug("envoyerIdent:Debut\n");
    envoyerUInt_32(ident.identifiant.sin_addr.s_addr, to);
    envoyerUInt_32(ident.identifiant.sin_port, to);
    envoyerUInt_32(ident.identifiant.sin_family,to);
    envoyerChaine(ident.name,to);
    envoyerHash(ident.h,to);
    envoyerUInt_32(ident.taille_hashtab,to);
    print_debug("envoyerIdent:Fin\n");
    return 1;
}

/*
 *reception de l'identité du client ou serveur
 */
static int recevoirIdent(idConnexion_t* ident, socket_t from)
{
    in_addr_t ip;
    uint32_t port;
    uint32_t protocol;
    char *name;
    uint64_t h;
    uint32_t taille_hashtab;
    struct sockaddr_in info;
	
    print_debug("recevoirIdent:Debut\n");
    recevoirUInt_32(&ip,from);
    recevoirUInt_32(&port,from);
    recevoirUInt_32(&protocol,from);
    recevoirChaine(&name,from);
    recevoirHash(&h,from);
    recevoirUInt_32(&taille_hashtab,from);
	
    info.sin_addr.s_addr = ip;
    info.sin_port = port;
    info.sin_family = (short)protocol;
	
    *ident = setIdConnexion(name,info, h, taille_hashtab);
    free(name);
    print_debug("recevoirIdent:Fin\n");
    return 1;
	
	
}


#endif

