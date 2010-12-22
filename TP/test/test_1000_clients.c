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


int main(int argc, char *argv[]) {
    int i;
    int num = atoi(argv[1]);
    printf("%d\n ",num);
    for(i = 0; i < num; i++) {
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, routine_t , NULL) != 0) {
            perror("error");
        }
     
    }
    sleep(10);
    return 0;
}

