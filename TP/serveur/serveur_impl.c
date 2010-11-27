#include "serveur_impl.h"

serveur_s create_serveur(char* serveurname, uint64_t port,
                         uint64_t first_k, uint64_t last_k,
                         idServeur_s next) {
    serveur_s new_s;
    uint64_t size_l = last_k - first_k + 1;
    hash_s* tab[size_l];
    int i;
    for (i = 0; i < size_l ; i++) {
        tab[i] = NULL;
    }
    new_s.s.serveurname = serveurname;
    new_s.s.port = port;
    new_s.size = size_l;
    new_s.firstKey = first_k;
    new_s.tabl = tab;
    new_s.next_serv = next;
    return new_s;
}


void *talk_to_client(void * donne) {

    int idSocket = (int) donne;
    /*    message_t ms;

    bzero(ms.data,MESSAGE_SIZE);
    strcpy(ms.data,"Tp DHT, mathias ramzi yongzhi");
    strcat(ms.data,"\n");
    send(idSocket,ms.data,MESSAGE_SIZE,0);
    bzero(ms.data,MESSAGE_SIZE);	

    int longueur = recv(idSocket,ms.data,MESSAGE_SIZE,0);
    if (longueur < 0) {

        printf("Serveur Recieve Data Failed!\n");
        exit(1);
    }
    printf("\nSocket Num: %d \t %s",idSocket, ms.data);
*/
    shutdown(idSocket, 2);
    pthread_exit(NULL); 
}


int put_h(serveur_s s, uint64_t cle, char * valeur, uint64_t taille) {
    char str[taille];
    hash_s * curr,* prec,* newCell;
    int i;
    uint64_t key_l = cle - s.firstKey;
    newCell = malloc(sizeof(char) * taille + sizeof(taille) + sizeof(hash_s*));
    for( i = 0; i < taille ; i++) {
        str[i] = valeur[i];
    }
    prec = s.tabl[key_l];
    curr = prec;
    while( curr != NULL ) {
        prec = curr;
        curr = curr->suiv;
    }
    newCell->size = taille;
    newCell->value = str;
    newCell->suiv = NULL;
    return 0;
}

uint64_t get_h(serveur_s s, uint64_t cle, char * valeur) {
    uint64_t key_l = cle - s.firstKey;    
    hash_s * curr = s.tabl[key_l];
    while( curr != NULL && strcmp(curr->value, valeur)) {
        curr = curr->suiv;
    }
    if (curr == NULL) { 
        valeur = NULL;
        return 0;
    }
    else {
        valeur = curr->value;
        return curr->size;
    }
}



int remove_h(serveur_s s, uint64_t cle, char* valeur, uint64_t taille) {
    int key_l = cle - s.firstKey;
    hash_s * curr, *prec;
    curr = s.tabl[key_l]; 
    if (curr != NULL && curr->size == taille && !strcmp(curr->value, valeur)) {
        s.tabl[key_l] = curr->suiv;
        free(curr);
        return 0;
    }
    else {
        while( curr != NULL && curr->size != taille && !strcmp(curr->value, valeur)) {
            prec = curr;
            curr = curr->suiv;
         }
        if (curr == NULL) {
            return -1;
        } 
        else {     
            prec->suiv = curr->suiv; 
            free(curr); 
            return 0; 
        }      
    } 
}


