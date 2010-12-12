#include "serveur_impl.h"

extern table_de_hachage_t TEST_HASH_TABLE();

int main(int argc, char *argv[])
{

    if (argc < 3) {
        printf("Usage: %s nomDuServeur Port\n", argv[0]);
        exit(0);
    }

    int nbClient = 0;
    tabClient_t p;
    serveur_t* serveur_ptr;
    socket_t sockClient;
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(struct sockaddr_in);
    char ip[20];
    int po;
    origine_t from;

    //creation d'un serveur
    serveur_ptr = creerServeur(argv[1], atoi(argv[2]));

    //connexion à un serveur pour participer à la DHT
    printf("se connecter à un serveur?[o|n]\n");
    if (getchar() == 'o') {
        printf("entrer l'adresse ip et le port du serveur'\n");
        scanf("%s", ip);
        scanf("%d", &po);
        printf("ip: %s et port: %d'\n", ip, po);

        if (!message_connect_2_server(ip, po)) {
            exit(-1);
        }
    } else {
        printf("vous êtes le premier serveur de la DHT\n");
        serveur_ptr->tabl=TEST_HASH_TABLE();
    }
	

    while (nbClient < LENGTH_LISTEN_QUEUE) {
        /* 
         * doit utiliser des pthread 
         * creation d'un thread à chaque fois que la demande de connexion 
         * d'un client est acceptée
         */
        printf("serveur en ecoute\n");
        sockClient = accept(serveur_ptr->idSocket, (struct sockaddr *)&cli_addr,&cli_len);
        recevoirOrigine(&from, sockClient);

        switch (from) {

        case FROM_CLIENT:

            /*
             * remplissage du tableau permettant d'itentifier les clients connectés
             */
            p = malloc(sizeof(tabClient_t));
            p->client.identifiant = cli_addr;
            p->client.idSocket = sockClient;
            nbClient++;

            printf("le client vient de se connecter au serveur\n");
            pthread_t client_thread=p->client.thread;
            
            p->suiv = serveur_ptr->tableauClient;
            serveur_ptr->tableauClient = p;


            if (pthread_create(&client_thread, NULL, *talk_to_client,(void *)&sockClient) < 0) {
                perror("Creation d'un nouveau pthread impossible \n");
                break;
            }
            printf("le client est validé : pthread crée\n");
            break;

        case FROM_SERVEUR:

            printf("un serveur vient de se connecter au serveur\n");

            /* creation d'un thread pour repondre aux requetes du serveur */
            pthread_t server_thread;
            if (pthread_create(&server_thread, NULL, *talk_to_server,(void *)&sockClient) < 0) {
                perror("Creation d'un nouveau pthread impossible \n");
                break;
            }
            printf("le serveur est validé : pthread crée\n");
            break;

        default:

            printf("type d'expediteur inconnu\n");

        }

    }

    printf("votre serveur n'accepte pas plus de %d connexions à la fois\n",
           LENGTH_LISTEN_QUEUE);

    return 0;

}

