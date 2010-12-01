#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/socket.h>
#include "commun.h"




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

enum ___type_requete {
    VALEUR,
    OCTET,
    CHAINE,
    DONNEE,
    AUTRE,
};


union __message {

    char *Chaine;
    char Octet;
    donnee_t Donnee;
};


typedef struct message {

    enum ___type_message type_mess;
    enum ___type_requete type_requete;
    size_t taille;
    union __message mess;

} message_t;



static inline ssize_t
envoyerMessage(message_t message, size_t taille, socket_t destinataire)
{
    return send(destinataire, &message.mess, taille, 0);
}


static inline ssize_t
recevoirMessage(message_t message, size_t taille,socket_t expediteur)
{
    return recv(expediteur, &message.mess, taille, 0);
}




static ssize_t envoyerOctet(char O, socket_t to)
{
    SET_MESSAGE(M,REQUETE,OCTET,T_OCTET);
    M.mess.Octet = O;
    return envoyerMessage(M, M.taille, to);
}



static ssize_t envoyerChaine(char *chaine, socket_t to)
{
    SET_MESSAGE(M,REQUETE,CHAINE,T_CHAINE(chaine));
    M.mess.Chaine = chaine;
    return envoyerMessage(M, M.taille, to);
}

/* envoyer une valeur ou une cle revient à envoyer une chaine de caractere*/
#define envoyerValeur(valeur,to)  envoyerChaine(valeur,to)
#define envoyerCle(cle,to)        envoyerChaine(cle,to)


static ssize_t envoyerDonnee(donnee_t D, socket_t to)
{
  return envoyerChaine(D->cle,to)+envoyerChaine(D->valeur,to);
}

#endif

