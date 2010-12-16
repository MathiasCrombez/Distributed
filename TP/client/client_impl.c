#include"client_impl.h"


#define SET_CLIENT_NAME(NAME)                                           \
        do {                                                            \
                strcpy(CLIENT.name,NAME);                               \
        } while(0)






client_t *creerClient(const char *nom)
{
	SET_CLIENT_NAME(nom);
	return &CLIENT;
}



/*
 * Connect à un server.Renvoi le socket qui permet de communiquer avec le serveur
 */
socket_t ___connect2server___(struct sockaddr_in server_info)
{
        socket_t idSocket;

        idSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (idSocket < 0) {
                perror("socket()");
                exit(-1);
        }

        if (connect(idSocket, (struct sockaddr *)&server_info,sizeof(struct sockaddr_in)) == -1) {
                printf("connect2server:Echec de la connexion\n");
                // TODO répétition en cas d'echec : wait & while
                exit(-1);
        }
        CLIENT.idSocket = idSocket;
        return idSocket;
}


