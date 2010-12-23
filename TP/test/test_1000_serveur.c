#include "serveur_impl.h"



int main(int argc, char *argv[])
{

        int nbClient = 0;
        tabClient_t p;
        serveur_t *serveur_ptr;
        socket_t sockClient;
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(struct sockaddr_in);
 
        origine_t from;
        pthread_t client_thread;
        int i=1;
        //creation d'un serveur
        serveur_ptr = creerServeur("ServeurTest1", 4242);

  
        while (nbClient<atoi(argv[1])) {
                /* 
                 * creation d'un thread à chaque fois que la demande de connexion 
                 * d'un client est acceptée
                 */
                printf("serveur en ecoute\n");
                sockClient = accept(serveur_ptr->idSocket, (struct sockaddr *)&cli_addr,&cli_len);
                if(sockClient==-1){
                        perror("accept()");
                        exit(-1);
                }
                
                printf("%d\n",nbClient);
                nbClient++;
                recevoirOrigine(&from, sockClient);

                switch (from) {

                case FROM_CLIENT:

                    
                        if (pthread_create(&client_thread, NULL, *talk_to_client, (void *)&sockClient) < 0) {
                                perror("KO \n");
                                break;
                        }
                        printf("OK\n");

                        break;

               
                default:

                        printf("type d'expediteur inconnu\n");

                }

        }

        printf("trop de clients!");
        return 0;

}

