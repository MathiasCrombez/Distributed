#include "serveur_impl.h"

int main(int argc, char *argv[])
{

	if (argc < 3) {
		printf("Usage: %s nomDuServeur Port\n", argv[0]);
		exit(0);
	}

	int nbClient = 0;
	tabClient_t p;
	serveur_t *serveur_ptr;
	socket_t sockClient;
	struct sockaddr_in cli_addr;
	socklen_t cli_len = sizeof(struct sockaddr_in);
	char ip[20];
	int po;
	origine_t from;
	pthread_t client_thread;

	//creation d'un serveur
	serveur_ptr = creerServeur(argv[1], atoi(argv[2]));

	//connexion à un serveur pour participer à la DHT
	printf("se connecter à un serveur?[o|n]\n");
	if (getchar() == 'o') {
		printf("entrer le port du serveur'\n");
		// scanf("%s", ip);
		scanf("%d", &po);
		//printf("ip: %s et port: %d'\n", ip, po);

		if (!message_connect_2_server("localhost", po)) {
			exit(-1);
		}
	} else {
		printf("vous êtes le premier serveur de la DHT\n");
		serveur_ptr->tabl = TEST_HASH_TABLE();
	}

	while (nbClient < LENGTH_LISTEN_QUEUE) {

		printf("serveur en ecoute\n");

		if (SERVER_IS_DYING_VAR == 1) {
			printf("jdcfrfr\n");
			//exit(0);
		}

		sockClient =
		    accept(serveur_ptr->idSocket, (struct sockaddr *)&cli_addr,
			   &cli_len);
		if (sockClient == -1) {
			perror("accept()");
			exit(-1);
		}

		recevoirOrigine(&from, sockClient);

		switch (from) {

		case FROM_CLIENT:

			/*
			 * remplissage du tableau permettant d'itentifier les clients connectés
			 */

			client_thread =
			    preconnect_serv2cli(cli_addr, sockClient,
						&serveur_ptr);

			nbClient++;

			printf("le client vient de se connecter au serveur\n");

			if (pthread_create
			    (&client_thread, NULL, *talk_to_client,
			     (void *)sockClient)
			    < 0) {
				perror
				    ("Creation d'un nouveau pthread impossible \n");
				break;
			}
			printf("le client est validé : pthread crée\n");
			break;

		case FROM_SERVEUR:

			printf("un serveur vient de se connecter au serveur\n");

			/* creation d'un thread pour repondre aux requetes du serveur */
			pthread_t server_thread;
			if (pthread_create
			    (&server_thread, NULL, *talk_to_server,
			     (void *)sockClient)
			    < 0) {
				perror
				    ("Creation d'un nouveau pthread impossible \n");
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

