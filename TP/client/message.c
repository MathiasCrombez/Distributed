#include"message.h"
#include "client_impl.h"


#define DEBUG_MESSAGE_CLIENT

socket_t message_connect(idConnexion_t * server){
    char ack;
    socket_t from; 

    from = connect2server(server);
        
    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(CONNECT,from);
    recevoirOctet(&ack, from);
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
    idConnexion_t * ident;
    socket_t new_from;
    
    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(GET,from);
    envoyerCle(K,from);
    recevoirOctet(&ack,from);
	
    if(ack==0){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("la cle n'existe pas\n");
#endif
        /*
         * On relance la fonction avec un nouveau serveur et une nouvelle socket
         */
        recevoirIdent(&ident, from);
        message_disconnect(from);
        shutdown(from, 2);
        new_from = message_connect(ident);
        return message_get(K, new_from);
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
    idConnexion_t * ident;
    socket_t new_from;
	
    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(PUT,from);
    envoyerCle(K,from);
    recevoirOctet(&ack,from);
	
    if(ack==0){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("la cle n'existe pas\n");
#endif
        /*
         * On relance la fonction avec un nouveau serveur et une nouvelle socket
         */
        recevoirIdent(&ident, from);
        message_disconnect(from);
        shutdown(from, 2);
        new_from = message_connect(ident);
        message_put(K, new_from);
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
    idConnexion_t * ident;
    socket_t new_from;
	
    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(REMOVEKEY,from);
    envoyerCle(K,from);
    recevoirOctet(&ack,from);
	
    if(ack==0){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("la cle n'existe pas\n");
#endif
        /*
         * On relance la fonction avec un nouveau serveur et une nouvelle socket
         */
        recevoirIdent(&ident, from);
        message_disconnect(from);
        shutdown(from, 2);
        new_from = message_connect(ident);
        return message_remove(K, new_from);
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
