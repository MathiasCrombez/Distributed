#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

uint32_t PORT = 0;

int main()
{
    int idSocket, newIdSocket, clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if ( (idSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("erreur socket");
	exit(EXIT_FAILURE);
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(idSocket, (struct sockaddr *) &serv_addr,
	     sizeof(serv_addr)) < 0) {
	printf("erreur bind");
	exit(EXIT_FAILURE);
    }
    listen(idSocket,5);
    clilen = sizeof(cli_addr);
    newIdSocket = accept(idSocket, 
			 (struct sockaddr *) &cli_addr, 
			 &clilen);
    if (newIdSocket < 0) {
	printf("ERROR on accept");
	exit(EXIT_FAILURE);
    }
    return 0; 
}
