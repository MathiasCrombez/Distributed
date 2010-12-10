#include "client.h"

int main(int argc, char *argv[])
{
	if (argc < 4) {
		printf("Usage: %s nomDuClient nomDuServeur numeroPort\n",
		       argv[0]);
		exit(0);
	}

	client_t client;
	char o;
	donnee_t d;
	
	client = creerClient(argv[1]);
	connect2server(client, argv[2], atoi(argv[3]));
	
	envoyerOrigine(FROM_CLIENT,client.idSocket);
	envoyerTypeMessage(GET,client.idSocket);
	envoyerCle("d",client.idSocket);
	recevoirOctet(&o,client.idSocket);
	if(o==0){
		printf("la cle n'existe pas");
	}
	else if(o==1){
		recevoirDonnee(&d,client.idSocket);
		afficherDonnee(d);
	}
	return 1;
}

