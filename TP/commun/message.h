#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/socket.h>
 #include <sys/types.h> 
 #include <arpa/inet.h>
#include "commun.h"
#define OUI 1
#define NON 0



#define T_VALEUR 		sizeof(valeur_t)
#define T_OCTET 		sizeof(char)
#define T_CHAINE(string) strlen(string)*T_OCTET
#define T_DONNEE(donnee)   T_CHAINE(donnee->cle) + T_CHAINE(donnee->valeur)


#define SET_MESSAGE(M,type_m,type_r,len)                               \
    message_t M;                                                       \
    M.type_mess = type_m;                                              \
    M.type_requete = type_r;                                           \
    M.taille = len;                                                    \

enum ___type_message {
    REQUETE,
    REPONSE,
};

typedef enum ___type_requete {
    PUT,
    GET,
    ACK,
    IDENT,
    CONNECT,
}requete_t;


union __message {

  
};


typedef struct message {

    enum ___type_message type_mess;
    enum ___type_requete type_requete;
    size_t taille;
    union __message mess;

} message_t;






static int envoyerUInt_32(uint32_t I,socket_t to ){

    char* s_I= malloc(sizeof(uint32_t));
    sprintf(s_I,"%d",I);
    printf("ent: %s \n",s_I);
    send(to,s_I,sizeof(uint32_t),0);
    return 1;
}

static int recevoirUInt_32(uint32_t* I,socket_t from){

    char* s_I= malloc(sizeof(uint32_t));
    recv(from,s_I,sizeof(uint32_t),0);
    *I=atoi(s_I);
    return 1;
}


static int envoyerOctet(char O, socket_t to)
{
    send(to,&O,T_OCTET,0);
    return 1;
}

static int recevoirOctet(char* O,socket_t from){

    recv(from,O,T_OCTET,0);
}

static int envoyerChaine(char *chaine, socket_t to)
{
    uint32_t taille_chaine = T_CHAINE(chaine);
    
    envoyerUInt_32(taille_chaine,to);
    send(to,chaine,taille_chaine,0);
    return 1;
}

static int recevoirChaine(char** chaine, socket_t from){

    uint32_t taille_chaine;
    
    recevoirUInt_32(&taille_chaine,from);
    
    *chaine = (char*)malloc(T_OCTET*taille_chaine);
    
    recv(from,*chaine,taille_chaine,0);
    return 1;
}
/* envoyer une valeur ou une cle revient à envoyer une chaine de caractere*/
#define envoyerValeur(valeur,to)  envoyerChaine(valeur,to)
#define envoyerCle(cle,to)        envoyerChaine(cle,to)

#define recevoirValeur(valeur,to)  recevoirChaine(valeur,to)
#define recevoirCle(cle,to)        recevoirChaine(cle,to)

static int envoyerDonnee(donnee_t D, socket_t to)
{
  return envoyerChaine(D->cle,to)+envoyerChaine(D->valeur,to);
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

    envoyerUInt_32(ident.sin_addr.s_addr,to);
    envoyerUInt_32(ident.sin_port,to);
//    envoyerUInt_32(ident.sin_family,to);
    return 1;
}

static int recevoirIdent(struct sockaddr_in* ident , socket_t from){

    recevoirUInt_32(&(ident->sin_addr.s_addr),from);
    recevoirUInt_32(&(ident->sin_port),from);
//    recevoirUInt_32(&(ident->sin_family),from);
    
    return 1;
}
#endif

