#include "serveur_impl.h"

serveur_s create_serveur(char* serveurname, uint64_t port,
                         uint64_t first_k, uint64_t last_k) {
    serveur_s s;
    uint64_t size_l = last_k - first_k + 1;
    hash_s* tab[size_l];
    int i;
    for (i = 0; i < size_l ; i++) {
        tab[i] = NULL;
    }
    s.serveurname = serveurname;
    s.port = port;
    s.size = size_l;
    s.firstKey = first_k;
    s.tabl = tab;
    
   
    return s;
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


