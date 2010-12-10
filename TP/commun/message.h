#ifndef MESSAGE_H
#define MESSAGE_H


#include "commun.h"

#define OUI 1
#define NON 0



#define T_VALEUR 		   sizeof(valeur_t)
#define T_INT_32 		   10
#define T_INT_64 		   20
#define T_OCTET 		   sizeof(char)
#define T_CHAINE(string)   strlen(string)*T_OCTET
#define T_DONNEE(donnee)   T_CHAINE(donnee->cle) + T_CHAINE(donnee->valeur)

#define DEBUG_MESSAGE_H
#undef  DEBUG_MESSAGE_H

#ifdef DEBUG_MESSAGE_H
#   define print_debug(chaine, args...)   printf(chaine,##args)
#else
#   define print_debug(chaine, args...)
#endif
    

//#define SET_MESSAGE(M,type_r,len)                                      \
//    message_t M;                                                       \
//                                                                       \
//    M.type_requete = type_r;                                           \
//    M.taille = len;                                                    \


//enum ___type_message {
//    REQUETE,
//    REPONSE,
//};

typedef enum ___type_requete {
    PUT,
    GET,
    ACK,
    IDENT,
    CONNECT,
    DISCONNECT,
}requete_t;


//union __message {

//  
//};



static int envoyerUInt_32(uint32_t I,socket_t to ){

    char* s_I= malloc(T_INT_32);
    if(s_I == NULL){
        perror("malloc()");
        return 0;
    }
    sprintf(s_I,"%d",I);
    
    if(send(to,s_I,T_INT_32,0)==-1){
        perror("send()");
        return 0;   
    }
    print_debug("envoi de: %d\n",I);
    return 1;
}

static int recevoirUInt_32(uint32_t* I,socket_t from){

    char* s_I= malloc(T_INT_32);
    if(s_I == NULL){
        perror("malloc()");
        return 0;
    }
    
    if(recv(from,s_I,T_INT_32,0)==-1){
        perror("recv()");
        return 0;
    }
    *I=atoi(s_I);
    print_debug("recep de: %d\n",*I);
    return 1;
}


static envoyerUInt_64(uint64_t I, socket_t to){

    char* s_I= malloc(T_INT_64);
    if(s_I == NULL){
        perror("malloc()");
        return 0;
    }
    sprintf(s_I,"%ld",I);
    
    if(send(to,s_I,T_INT_64,0)==-1){
        perror("send()");
        return 0;   
    }
    print_debug("envoi de: %ld\n",I);
    return 1;
}


static recevoirUInt_64(uint64_t* I, socket_t from)
{
    char* s_I= malloc(T_INT_64);
    if(s_I == NULL){
        perror("malloc()");
        return 0;
    }
      if(recv(from,s_I,T_INT_64,0)==-1){
        perror("recv()");
        return 0;
    }
    *I=atoi(s_I);
    print_debug("recep de: %ld\n",*I);
    return 1;
}


static int envoyerOctet(char O, socket_t to)
{ 
    if(send(to,&O,T_OCTET,0)==-1){
        perror("send");
        return 0;
    }
    print_debug("envoi de: %c\n",O);
    return 1;
}

static int recevoirOctet(char* O,socket_t from)
{
    if(recv(from,O,T_OCTET,0)==-1){
        perror("recv()");
        return 0;
    }
    print_debug("recep de: %c\n",*O);
    return 1;
}

static int envoyerChaine(char *chaine, socket_t to)
{
    uint32_t taille_chaine = T_CHAINE(chaine);
    print_debug("\t:");
    if(!envoyerUInt_32(taille_chaine,to)){
        return 0;
    }
    print_debug("\t:");
    if(send(to,chaine,taille_chaine,0)==-1){
        perror("send()");
        return 0;
    }
    print_debug("envoi de: %s\n",chaine);
    return 1;
}

static int recevoirChaine(char** chaine, socket_t from){

    uint32_t taille_chaine;
    print_debug("\t:");
    if(!recevoirUInt_32(&taille_chaine,from)){
        return 0;
    }
    print_debug("\t:");
    *chaine = (char*)malloc(T_OCTET*taille_chaine);
    if(*chaine==NULL){
        perror("malloc");
        return 0;
    }
    if(recv(from,*chaine,taille_chaine,0)==-1){
        perror("recv()");
        return 0;
    }
    print_debug("recep de: %s\n",*chaine);
    return 1;
}
/* envoyer une valeur ou une cle revient à envoyer une chaine de caractere*/
#define envoyerValeur(valeur,to)  envoyerChaine(valeur,to)
#define envoyerCle(cle,to)        envoyerChaine(cle,to)

#define recevoirValeur(valeur,to)  recevoirChaine(valeur,to)
#define recevoirCle(cle,to)        recevoirChaine(cle,to)

static int envoyerDonnee(donnee_t D, socket_t to)
{
  return (envoyerChaine(D->cle,to) & envoyerChaine(D->valeur,to));
}


static int recevoirDonnee(donnee_t* D, socket_t from){
    
    cle_t cle;
    valeur_t val;
    
    recevoirCle(&cle,from);
    recevoirValeur(&val,from);
    
    *D=creerDonnee(cle,val);
    return 1;
}

static int envoyerTypeMessage(requete_t I,socket_t to){

    return envoyerUInt_32(I,to);
}

static int recevoirTypeMessage(requete_t* I,socket_t from){
    return recevoirUInt_32(I,from);
}


static int envoyerIdent(struct sockaddr_in ident,socket_t to){

    envoyerChaine(inet_ntoa(ident.sin_addr),to);
    envoyerUInt_32(ident.sin_port,to);
//    envoyerUInt_32(ident.sin_family,to);
    return 1;
}

static int recevoirIdent(struct sockaddr_in* ident , socket_t from){

	char* ip;
	
    recevoirChaine(&ip,from);
    recevoirUInt_32(&(ident->sin_port),from);

	ident->sin_addr.s_addr = inet_addr(ip);
    
    return 1;
}


static int envoyerHash(uint64_t h,socket_t to){

    return envoyerUInt_64(h,to);
}

static int recevoirHash(uint64_t* h, socket_t from){

    return recevoirUInt_64(h,from);
}
#endif

