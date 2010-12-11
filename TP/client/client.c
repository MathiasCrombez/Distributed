#include "client_impl.h"




int main(int argc, char *argv[])
{
	if (argc < 4) {
		printf("Usage: %s nomDuClient nomDuServeur numeroPort\n",
		       argv[0]);
		exit(0);
	}

	client_t* client_ptr;
	socket_t sockServer;
	char o;
	donnee_t d;
	
	client_ptr = creerClient(argv[1]);
	sockServer = connect2server(argv[2], atoi(argv[3]));
	
	message_get("d",sockServer);
	
	return 1;
}

