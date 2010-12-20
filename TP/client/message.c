#include"message.h"

#define DEBUG_MESSAGE_CLIENT

/*
 * renvoi une structure sockaddr_in avec l'ip et le port de connexion
 * protocole TCP/IP
 */
inline struct sockaddr_in ___get_sockaddr_in___(char *ip, uint32_t port)
{

        struct hostent *hostinfo;
        struct sockaddr_in serv_addr;

        hostinfo = gethostbyname(ip);
        if (hostinfo == NULL) {
                printf("___get_sockaddr_in__:Erreur gethostbyname()\n", ip);
                exit(-1);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr = *(struct in_addr *)hostinfo->h_addr;

        return serv_addr;
}


socket_t message_connect(char* ip, uint32_t port)
{

        struct sockaddr_in server_info;
        char ack;
        socket_t sockServer;
        
        server_info = ___get_sockaddr_in___(ip,port);
        sockServer  = ___connect2server___(server_info);

        envoyerOrigine(FROM_CLIENT,sockServer);
        envoyerTypeMessage(CONNECT,sockServer);
        recevoirOctet(&ack,sockServer);
        return sockServer;
}



void message_put(donnee_t D, socket_t from)
{

        char ack;
        idConnexion_t ident;
        socket_t new_from;

        
        envoyerTypeMessage(PUT, from);
        envoyerCle(D->cle, from);
        recevoirOctet(&ack, from);

        if (ack == 0) {
        #ifdef DEBUG_MESSAGE_CLIENT
                printf("message_put:serveur suivant\n");
        #endif
                /*
                 * On relance la fonction avec un nouveau serveur et une nouvelle socket
                 */
                recevoirIdent(&ident, from);
                message_disconnect(from);
                new_from =___connect2server___(ident.identifiant);
                message_put(D, new_from);
        } else if (ack == 1) {
                envoyerDonnee(D, from);
        #ifdef DEBUG_MESSAGE_CLIENT
                printf("message_put:\n");
                afficherDonnee(D);
        #endif
        }
}

donnee_t message_get(cle_t K, socket_t from)
{

        donnee_t D;
        char ack, ack2;
        idConnexion_t ident;
        socket_t new_from;

        envoyerTypeMessage(GET, from);
        envoyerCle(K, from);
        recevoirOctet(&ack, from);

        if (ack == 0) {
        #ifdef DEBUG_MESSAGE_CLIENT
                printf("message_get:serveur suivant\n");
        #endif
                /*
                 * On relance la fonction avec un nouveau serveur et une nouvelle socket
                 */
                recevoirIdent(&ident, from);
                message_disconnect(from);
                shutdown(from, 2);
                new_from = ___connect2server___(ident.identifiant);
                return message_get(K, new_from);
        } else if (ack == 1) {
                recevoirOctet(&ack2, from);
                if (ack2 == 0) {
                        printf("message_get:cle inconnue\n");
                } else {
                        recevoirDonnee(&D, from);
                #ifdef DEBUG_MESSAGE_CLIENT
                        printf("message_get:\n");
                        afficherDonnee(D);
                #endif
                }
        }
        return D;
}


valeur_t message_remove(cle_t K, socket_t from)
{
        donnee_t D;
        char ack, ack2;
        valeur_t V;
        idConnexion_t ident;
        socket_t new_from;

        //    envoyerOrigine(FROM_CLIENT,from);
        envoyerTypeMessage(REMOVEKEY, from);
        envoyerCle(K, from);
        //        recevoirOctet(&ack, from);

        if (ack == 0) {
#ifdef DEBUG_MESSAGE_CLIENT
                printf("message_remove:serveur suivant\n");
#endif
                /*
                 * On relance la fonction avec un nouveau serveur et une nouvelle socket
                 */
                recevoirIdent(&ident, from);
                message_disconnect(from);
                shutdown(from, 2);
                new_from = ___connect2server___(ident.identifiant);
                return message_remove(K, new_from);
        } else if (ack == 1) {
                recevoirOctet(&ack2, from);
                if (ack2 == 0) {
                        printf("message_remove;cle inconnue\n");
                } else {
                        recevoirValeur(&V, from);
#ifdef DEBUG_MESSAGE_CLIENT
                        printf("message_remove:valeur supprimee:%s\n", V);
#endif
                }
                return V;
        }
}

void message_disconnect(socket_t from)
{
    char ack;
        envoyerTypeMessage(DISCONNECT, from);
        recevoirOctet(&ack, from);
        close(from);
#ifdef DEBUG_MESSAGE_CLIENT
        printf("message_disconnect:deconnexion\n");
#endif
}

