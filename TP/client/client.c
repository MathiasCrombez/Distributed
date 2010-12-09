#include "client.h"

int main(int argc, char *argv[])
{
	if (argc < 4) {
		printf("Usage: %s nomDuClient nomDuServeur numeroPort\n",
		       argv[0]);
		exit(0);
	}

	client_t client;

	client = creerClient(argv[1]);
	connect2server(client, argv[2], atoi(argv[3]));
	envoyerUInt_32(35533, client.idSocket);
	envoyerChaine("salut serveur, c'est le client'", client.idSocket);
	donnee_t D = creerDonnee("une nouvelle cle", "range cette valeur dans ta dht");
	envoyerDonnee(D, client.idSocket);

	return 1;
}

