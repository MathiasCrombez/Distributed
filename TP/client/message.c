#include"message.h"
#include"client.h"





int put(donnee_t D,socket to){

    char reponse;
    socket_t from=to;
    
    envoyerTypeMessage(PUT,to); 
    recevoirOctet(&reponse,from);

    if(reponse==OUI){
        printf("put vers le bon serveur\n");
        envoyerDonnee(D,to);
        return 1;
    }
    else if(reponse ==NON){
        printf("vous ne contactez pas le bon serveur\n");
        return 1;
    }
    else {
        printf("reponse inconnue\n");
    }
    return 0;    
}

valeur_t  get(cle_t cle,socket from){



}

valeur_t removeKey(cle_t,socket to){



}



uint32_t disconnect2server(){



}
