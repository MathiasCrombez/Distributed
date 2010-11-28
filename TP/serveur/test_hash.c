#include "hash.h"
#include "commun.h"




int main(){


liste_t* l=creerHashTable(10);
//printf("taille %d\n",TAILLE_HASH_TABLE);

donnee_t data11,data12,data13,data2,data3;
data11= creerDonnee("ramzI","tu dois dormir");
data12= creerDonnee("ramzi","tu doiss dormir");
data13= creerDonnee("ramzi","tu doisss dormir");
data3=creerDonnee("ZAngh","pouje chwo tchang veun");
data2= creerDonnee("mathias","manga");
putHashTable(data11);
putHashTable(data12);
putHashTable(data13);

putHashTable(data3);
putHashTable(data2);
//printf("taille %d\n",TAILLE_HASH_TABLE);

printf("hash de ramzi :%ld\n",hash("ramzi"));


afficherListe(l[hash("ramzi")]);


//la clé n'est pas dans la dht
printf("donné enlevé: %s\n",removeHashTable("mathias"));
afficherListe(l[5]);

/*//la clé n'est pas dans la dht*/
/*printf("donné enlevé: %s\n",removeHashTable("ramzI"));*/

/*printf("liste apres remove ...\n");*/
/*afficherListe(l[hash("ramzi")]);*/

/*printf("donné enlevé: %s\n",removeHashTable("ZAngh"));*/
/*afficherListe(l[hash("ramzi")]);*/
}
