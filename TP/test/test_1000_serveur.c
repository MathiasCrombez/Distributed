#include "serveur_impl.h"

int main(int argc, char *argv[]) {
    int nbClient = 0;
    serveur_t *serveur_ptr;

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(struct sockaddr_in);
 

    origine_t from;
    pthread_t client_thread;
        socket_t  sockClient;
    //creation d'un serveur
    serveur_ptr = creerServeur("127.0.0.1", 4242);
    printf("Serveur opérationnel\n");
    while (nbClient < 100) {
        /* 
         * creation d'un thread à chaque fois que la demande de connexion 
         * d'un client est acceptée
         */


        if ( (sockClient 
              = accept(serveur_ptr->idSocket, (struct sockaddr *)&cli_addr,&cli_len))
             < 0) {
            perror("accept()");
        }
        /* printf("Un client se connecte avec la socket %d de %s:%d\n" */
        /*        , sockClient, inet_ntoa(cli_addr.sin_addr), htons(cli_addr.sin_port)); */
        recevoirOrigine(&from, sockClient);
        switch (from) {
        case FROM_CLIENT:
            /*
             * remplissage du tableau permettant d'itentifier les clients connectés
             */
            client_thread = preconnect_serv2cli(cli_addr, sockClient, &serveur_ptr);
            nbClient++;
            if (pthread_create(&client_thread, NULL, *talk_to_client, 
                               (void *)sockClient) != 0) {
                perror("KO \n");
            }
            break;               
        default:
            printf("type d'expediteur inconnu\n");
            break;
        }
    }
    return 0;
}

