#include "serveur_impl.h"

int put_h(serveur_s s, uint64_t cle, char * valeur, uint64_t taille) {
    hash_s * tab;
    hash_s * curr, * prec;
    curr = s.tabl;
    while( curr != NULL) {
        if (curr->key == cle) {
            return -1;
        }
        prec = curr;
        curr = curr->suiv;
    }
    tab = malloc(sizeof(cle) + sizeof(char) * taille);
    tab->key = cle;
    tab->size = taille;
    tab->value = valeur;
    tab->suiv =  NULL;
    prec->suiv = tab;
    return 0;
}

uint64_t get_h(serveur_s s, uint64_t cle, char * valeur) {
    hash_s * curr;
    curr = s.tabl;
    while( curr != NULL && curr->key != cle) {
        curr = curr->suiv;
    }
    if (curr == NULL) {
        // ??????
        valeur = NULL;
        return 0;
    }
    else {    
        valeur = curr->value;
        return curr->size;
    }
}



int remove_h(serveur_s s, uint64_t cle) {
    hash_s * curr, *prec;
    curr = s.tabl;
    if (curr != NULL && curr->key == cle) {
        s.tabl = curr->suiv;
        free(curr);
        return 0;
    }
    else {
        while( curr != NULL && curr->key != cle) {
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
