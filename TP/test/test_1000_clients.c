#include "client_impl.h"


int main(int argc, char *argv[])
{
    socket_t f;
    creerClient(argv[1]);
    f = message_connect("localhost",4242);
    sleep(2);
    message_disconnect(f);
    return 0;
}
