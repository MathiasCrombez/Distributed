#include "serveur_impl.h"
//gcc -o serveur serveur.c -lpthread

int main()
{
    struct sockaddr_in serv_addr, cli_addr;   
    int idSocket, cliIdSocket ;
    socklen_t length = sizeof(cli_addr);

    if ( (idSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("erreur socket");
        exit(EXIT_FAILURE);
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    printf("%d\n",INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if (bind(idSocket, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) {
        printf("erreur bind");
        exit(EXIT_FAILURE);
    }
    
    if(listen(idSocket,LENGTH_LISTEN_QUEUE)) {
        printf("Probleme de listenning");
    }

    while(1) {

        if ( (cliIdSocket = accept(idSocket, 
                                   (struct sockaddr *) &cli_addr, &length)) < 0) {
            printf("ERROR on accept");
            break;
        }
        pthread_t child_thread;
        pthread_attr_t child_thread_attr;
        pthread_attr_init(&child_thread_attr);
        pthread_attr_setdetachstate(&child_thread_attr,PTHREAD_CREATE_DETACHED);
        if( pthread_create(&child_thread,
                           &child_thread_attr,
                           talk_to_client, 
                           (void *)idSocket) < 0 ) {
            printf("pthread_create Failed : %s\n",strerror(errno));
        }
    }

    shutdown(idSocket, 2);
    return 0; 
}



/*  int idSocket, cliIdSocket, clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if ( (idSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("erreur socket");
    exit(EXIT_FAILURE);
    }
    memset((char *) &serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    printf("%d\n",INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if (bind(idSocket, (struct sockaddr *) &serv_addr,
    sizeof(serv_addr)) < 0) {
    printf("erreur bind");
    exit(EXIT_FAILURE);
    }
    listen(idSocket,5);
    clilen = sizeof(cli_addr);
    if ( cliIdSocket = accept(idSocket, (struct sockaddr *) &cli_addr, &clilen) < 0) {
    printf("ERROR on accept");
    exit(EXIT_FAILURE);
    }
    //   printf("connection à %s", inet_ntoa(cli_addr.sin_addr.s_addr));
    shutdown(idSocket, 2);

*/


