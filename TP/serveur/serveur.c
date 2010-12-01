#include "serveur_impl.h"
#include "message.h"



int main(int argc, char* argv[])
{
    if (argc < 3){ 
    	printf("Usage: %s nomDuServeur Port\n",argv[0]);
    	exit(0);
    }
	
    int nbClient =0;
    serveur_t serveur;
    socket_t sockClient;
    socklen_t cli_len = sizeof(struct sockaddr_in);
    struct sockaddr_in cli_addr;
    
    serveur = creerServeur(argv[1],atoi(argv[2]));
    printf("nom du serveur : %s\n",serveur.s.serveurname);
    
    while(nbClient<LENGTH_LISTEN_QUEUE){
    	/* 
         * doit utiliser des pthread 
         * creation d'un thread à chaque fois que la demande de connexion 
         * d'un client est acceptée
         */
        sockClient = accept(serveur.idSocket, (struct sockaddr *) &cli_addr, &cli_len);
		
        /*
         * remplissage du tableau permettant d'itentifier les clients connectés
         */
        serveur.tableauClient[nbClient].client_addr= cli_addr;
        serveur.tableauClient[nbClient].idSocket= sockClient;
        nbClient ++;
        
        printf("le client vient de se connecter au serveur\n");


        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, *talk_to_client, (void*)sockClient) < 0) {
            perror("Creation d'un nouveau pthread impossible \n");
            break;
        }
        printf("le client est validé : pthread crée\n");

    }
	
    printf("votre serveur n'accepte pas plus de %d connexions à la fois\n"
           ,LENGTH_LISTEN_QUEUE);

    /*    while(1) {*/

    /*        if ( (cliIdSocket = accept(idSocket, */
    /*                                   (struct sockaddr *) &cli_addr, &length)) < 0) {*/
    /*            printf("ERROR on accept");*/
    /*            break;*/
    /*        }*/
    /*        pthread_t child_thread;*/
    /*        pthread_attr_t child_thread_attr;*/
    /*        pthread_attr_init(&child_thread_attr);*/
    /*        pthread_attr_setdetachstate(&child_thread_attr,PTHREAD_CREATE_DETACHED);*/
    /*        if( pthread_create(&child_thread,*/
    /*                           &child_thread_attr,*/
    /*                           talk_to_client, */
    /*                           (void *)idSocket) < 0 ) {*/
    /*            printf("pthread_create Failed : %s\n",strerror(errno));*/
    /*        }*/
    /*    }*/

    /*    printf("connection à %s\n", inet_ntoa(cli_addr.sin_addr));*/
    /*    */
    /*    message_t message;*/
    /*    recv(cliIdSocket, &message, sizeof(message), 0);*/
    /*    printf("message %d\n", message.type);*/

    /*    shutdown(idSocket, 2);*/
    return 0; 
}



