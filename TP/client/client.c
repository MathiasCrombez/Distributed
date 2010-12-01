#include "client.h"




int main(int argc, char *argv[])
{
    if (argc < 4) {
	printf("Usage: %s nomDuClient nomDuServeur numeroPort\n", argv[0]);
	exit(0);
    }

    client_t client;

    client = creerClient(argv[1]);
    connect2server(client, argv[2], atoi(argv[3]));

    return 1;
}

