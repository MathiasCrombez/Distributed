#include "client_impl.h"
#include "message.h"
#define COMPT 10000


extern void interpreteur();

int main()
{

	interpreteur();
//TODO C A METTRE DS UN FICHIER TEST
/*    int i;*/
/*    socket_t sockServer[COMPT];*/
/*    client_t client_ptr[COMPT];*/
/*    char num[5];*/
/*    donnee_t D;*/
/*    if (argc < 3) {*/
/*        printf("Usage: %s nomDuServeur numeroPort\n",*/
/*               argv[0]);*/
/*        exit(0);*/
/*    }*/
/*    for (i=0;i<COMPT;i++) {*/

/*        char clientname[15];*/
/*        // convert 123 to string [buf]*/
/*        sprintf(clientname, "client%d",i);	*/
/*        client_ptr[i] = creerClient(clientname);*/
/*        sockServer[i] = message_connect(*/
/*                                        preConnect(argv[1], atoi(argv[2])),*/
/*                                        client_ptr[i]*/
/*                                     );*/
/*        //        sprintf(num, "%d", i);	*/
/*        D = creerDonnee(clientname,clientname);*/
/*        afficherDonnee(D);*/
/*        message_put(D,sockServer[i],client_ptr[i]);*/
/*     //} */
/*     //printf("Il y a %d clients\n",COMPT); */
/*     //for (i=0; i<COMPT; i++) { */
/*       //  char clientname[15]; */
/*        sprintf(clientname, "client%d", i);	*/
/*         D = (donnee_t)message_get(clientname, sockServer[i],client_ptr[i]);*/
/*        printf("Client %d :GET:%s\n",i,D->valeur);*/
/*        message_remove(clientname, sockServer[i],client_ptr[i]);*/
/*        message_disconnect(sockServer[i],client_ptr[i]);*/
/*	*/
/*    }*/
/*    return 1;*/

}

