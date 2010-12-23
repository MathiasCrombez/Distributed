#include "client_impl.h"

void * routine_t() {
    socket_t f;
    char * name = "client";
    creerClient(name);
    f = message_connect("localhost",4242);
    sleep(1);
    message_disconnect(f);
    pthread_exit(NULL);
};

<<<<<<< HEAD
int main(int argc, char *argv[])
{
        
        socket_t s;
        donnee_t D;
        
        creerClient(argv[1]);
        s=message_connect("localhost",4242);
        D = creerDonnee("cle","valeur");
        message_put(D,s);
        printf("%s %d\n",argv[1],s);
        while(1){
                sleep(10);
        }
        
=======

int main(int argc, char *argv[]) {
    int i;
    int num = atoi(argv[1]);
    for(i = 0; i < num; i++) {
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, routine_t , NULL) != 0) {
            perror("error");
        }
     
    }
    sleep(10);
    return 0;
>>>>>>> feba6583548dcafe0d222e14862a2764bd4ebb35
}

