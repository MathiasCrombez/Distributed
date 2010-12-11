#include"message.h"
#include "client_impl.h"


#define DEBUG_MESSAGE_CLIENT

//TODO ICI DEFINIR LE PROTOCOLE DE COMMUNICATION SEULEMENT ICI!!!

socket_t message_connect(char* server,uint64_t port){
    char ack;
    socket_t from; 

    from = connect2server(server, port);
        
    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(CONNECT,from);
	
    if(ack==0){
        // TODO répétition en cas d'echec : wait & while
        printf("Echec de la connexion");
        return -1;
    }
    else if(ack==1){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("connection success");
#endif
    }
    return from;
}

donnee_t  message_get(cle_t K,socket_t from){

    donnee_t D;
    char ack;
	
    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(GET,from);
    envoyerCle(K,from);
    recevoirOctet(&ack,from);
	
    if(ack==0){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("la cle n'existe pas\n");
#endif
    }
    else if(ack==1){
        recevoirDonnee(&D,from);
#ifdef DEBUG_MESSAGE_CLIENT
        afficherDonnee(D);
#endif
    }
    return D;
}

void  message_put(cle_t K,socket_t from){

    donnee_t D;
    char ack;
	
    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(PUT,from);
    envoyerCle(K,from);
    recevoirOctet(&ack,from);
	
    if(ack==0){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("la cle n'existe pas\n");
#endif
    }
    else if(ack==1){
        envoyerDonnee(D,from);
#ifdef DEBUG_MESSAGE_CLIENT
        afficherDonnee(D);
#endif
    }
}

valeur_t message_remove(cle_t K,socket_t from){

    donnee_t D;
    char ack;
    valeur_t V;
	
    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(REMOVEKEY,from);
    envoyerCle(K,from);
    recevoirOctet(&ack,from);
	
    if(ack==0){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("la cle n'existe pas\n");
#endif
        return NULL;
    }
    else if(ack==1){
        recevoirValeur(&V, from);
#ifdef DEBUG_MESSAGE_CLIENT
        printf("Valeur supprime : %s\n", V);
#endif
        return V;
    }
}

void message_disconnect(socket_t from){
    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(DISCONNECT,from);
}
