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
        printf("message_connect:Echec de la connexion");
        return -1;
    }
    else if(ack==1){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("message_connect:connection success");
#endif
    }
    return from;
}

void  message_put(donnee_t D,socket_t from){
    
    char ack;
    idConnexion_t * ident;
    socket_t new_from;
	
    //    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(PUT,from);
    envoyerCle(D->cle,from);
    recevoirOctet(&ack,from);
	
    if(ack==0){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("message_put:serveur suivant\n");
#endif
        /*
         * On relance la fonction avec un nouveau serveur et une nouvelle socket
         */
        recevoirIdent(&ident, from);
        message_disconnect(from);
        shutdown(from, 2);
        new_from = message_connect(ident);
        message_put(D, new_from);
    }
    else if(ack==1){
        envoyerDonnee(D,from);
#ifdef DEBUG_MESSAGE_CLIENT
        printf("message_put:\n");
        afficherDonnee(D);
#endif
    }
}

donnee_t  message_get(cle_t K,socket_t from){

    donnee_t D;
    char ack, ack2;
    idConnexion_t * ident;
    socket_t new_from;
    
    //    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(GET,from);
    envoyerCle(K,from);
    recevoirOctet(&ack,from);
	
    if(ack==0){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("message_get:serveur suivant\n");
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
        recevoirOctet(&ack2,from);
        if (ack2==0) {
            printf("message_get:cle inconnue\n");
        }
        else {
            recevoirDonnee(&D,from);
#ifdef DEBUG_MESSAGE_CLIENT
            printf("message_get:\n");
            afficherDonnee(D);
#endif
        }
    }
    return D;
}

valeur_t message_remove(cle_t K,socket_t from){

    donnee_t D;
    char ack, ack2;
    valeur_t V;
    idConnexion_t * ident;
    socket_t new_from;
	
    //    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(REMOVEKEY,from);
    envoyerCle(K,from);
    recevoirOctet(&ack,from);
	
    if(ack==0){
#ifdef DEBUG_MESSAGE_CLIENT
        printf("message_remove:serveur suivant\n");
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
        recevoirOctet(&ack2,from);
        if (ack2==0) {
            printf("message_remove;cle inconnue\n");
        }
        else {
            recevoirValeur(&V, from);
#ifdef DEBUG_MESSAGE_CLIENT
            printf("message_remove:donnee supprimee %s\n",V);           
#endif
        }
        return V;
    }
}

void message_disconnect(socket_t from){
    //    envoyerOrigine(FROM_CLIENT,from);
    envoyerTypeMessage(DISCONNECT,from);
#ifdef DEBUG_MESSAGE_CLIENT
    printf("message_disconnect:deconnexion\n");
#endif    
}
