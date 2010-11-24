#include "serveur_impl.h"

serveur_s create_serveur(char* serveurname, uint64_t port,
                         uint64_t first_k, uint64_t last_k) {
    serveur_s s;
    uint64_t size_l = last_k - first_k + 1;
    hash_s tab[size_l];
    int i;
    for (i = 0; i < size_l ; i++) {
        tab[i].size = 0;
        tab[i].value = NULL;
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
    int i;
    for( i = 0; i < taille ; i++) {
        str[i] = valeur[i];
    }
    //s.tabl[cle] = cle;
    s.tabl[cle - s.firstKey].size = taille;
    *(s.tabl[cle - s.firstKey].value) = str;
    return 0;
}

uint64_t get_h(serveur_s s, uint64_t cle, char * valeur) {
    valeur = *(s.tabl[cle - s.firstKey].value);
    return s.tabl[cle - s.firstKey].size;


    /* hash_s * curr; */
    /* curr = s.tabl; */
    /* while( curr != NULL && curr->key != cle) { */
    /*     curr = curr->suiv; */
    /* } */
    /* if (curr == NULL) { */
    /*     // ?????? */
    /*     valeur = NULL; */
    /*     return 0; */
    /* } */
    /* else {     */
    /*     valeur = curr->value; */
    /*     return curr->size; */
    /* } */
}



int remove_h(serveur_s s, uint64_t cle) {
    s.tabl[cle - s.firstKey].size = 0;
    s.tabl[cle - s.firstKey].value = (char **)NULL;
    return 0;
    /* hash_s * curr, *prec; */
    /* curr = s.tabl; */
    /* if (curr != NULL && curr->key == cle) { */
    /*     s.tabl = curr->suiv; */
    /*     free(curr); */
    /*     return 0; */
    /* } */
    /* else { */
    /*     while( curr != NULL && curr->key != cle) { */
    /*         prec = curr; */
    /*         curr = curr->suiv; */
    /*     } */
    /*     if (curr == NULL) { */
    /*         return -1; */
    /*     } */
    /*     else {     */
    /*         prec->suiv = curr->suiv; */
    /*         free(curr); */
    /*         return 0; */
    /*     } */
    /* } */
}
