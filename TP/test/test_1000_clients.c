#include "client_impl.h"


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
        
}
