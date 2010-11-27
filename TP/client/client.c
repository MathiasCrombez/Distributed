#include "client.h"
#include <assert.h>

int main()
{

	client_t client;
	message_t message;

	client = creerClient("127.0.0.1", 0);
	connect2server(client);
	//printf("message : %d \n", message.type);
	envoyerMessage(message, client.idSocket);
	//printf("message : %d \n", message.type);
	//disconnect2server();
	//while(1);
	
	return 1;
}



