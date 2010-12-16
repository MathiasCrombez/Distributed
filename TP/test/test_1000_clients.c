#include "client_impl.h"


int main(int argc, char *argv[])
{

        creerClient(argv[1]);
        message_connect("localhost",4242);

        while(1){
                /* consomme moins de charge cpu*/
                sleep(1000);
        }
}
