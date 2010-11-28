#include "hash.h"
#include "commun.h"




int main(){


creerHashTable(10);

donnee_t data11,data12,data13,data2,data3;
data11= creerDonnee("ramzi","tu dois dormir");
data12= creerDonnee("ramzi","tu doiss dormir");
data13= creerDonnee("ramzi","tu doisss dormir");
data2= creerDonnee("mathias","manga");
data3=creerDonnee("ZAngh","pouje chwo tchang veun");
putHashTable(data11);
putHashTable(data12);
putHashTable(data13);
putHashTable(data2);
putHashTable(data3);
printf("%d taille %d\n",hash("ramzi"),TAILLE_HASH_TABLE);
//liste_t l = tableDeHachage[hash("ramzi")];
//afficherListe(;


}
