#include "serveur_impl.h"

int main(int argc, char *argv[])
{

	if (argc < 3) {
		printf("Usage: %s nomDuServeur Port\n", argv[0]);
		exit(0);
	}

	int nbClient = 0;
	serveur_t serveur;
	socket_t sockClient;
	struct sockaddr_in cli_addr;
	socklen_t cli_len = sizeof(struct sockaddr_in);
	char ip[20];
	int po;

	//creation d'un serveur
	serveur = creerServeur(argv[1], atoi(argv[2]));

	//connexion à un serveur pour participer à la DHT
	printf("se connecter à un serveur?[o|n]\n");
	if (getchar() == 'o') {
		printf("entrer l'adresse ip et le port du serveur'\n");
		scanf("%s", ip);
		scanf("%d", &po);
		printf("ip: %s et port: %d'\n", ip, po);

		if (!messageConnect2Server(ip, po)) {
			exit(-1);
		}
	} else {
		printf("vous êtes le premier serveur de la DHT\n");
		//messageParticiperDhtAvec(ip,to);
	}
	printf("serveur en ecoute\n");

	while (nbClient < LENGTH_LISTEN_QUEUE) {
		/* 
		 * doit utiliser des pthread 
		 * creation d'un thread à chaque fois que la demande de connexion 
		 * d'un client est acceptée
		 */
		sockClient =accept(serveur.idSocket, (struct sockaddr *)&cli_addr,&cli_len);

		/*
		 * remplissage du tableau permettant d'itentifier les clients connectés
		 */
		serveur.tableauClient[nbClient].identifiant = cli_addr;
		serveur.tableauClient[nbClient].idSocket = sockClient;
		nbClient++;

        printf("le client vient de se connecter au serveur\n");

		pthread_t client_thread;
		if (pthread_create(&client_thread, NULL, *talk_to_client,(void *)sockClient) < 0) {
			perror("Creation d'un nouveau pthread impossible \n");
			break;
		}
		printf("le client est validé : pthread crée\n");

    }

	printf("votre serveur n'accepte pas plus de %d connexions à la fois\n",LENGTH_LISTEN_QUEUE);

	return 0;
}

