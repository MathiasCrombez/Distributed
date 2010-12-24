#include "hash.h"
#include "commun.h"

int main()
{

	table_de_hachage_t table;
	table = TEST_HASH_TABLE();


	donnee_t d1, d2, d3, d4, d5, d6, d7;

	d1 = creerDonnee("ramzi", "souris");
	d2 = creerDonnee("ramziiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii","souris");
	d3 = creerDonnee("ramzi","souriiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiis");
	d4 = creerDonnee("ramzi", "verte");
	d5 = creerDonnee("ramzi", "je t'attrape'");
	d6 = creerDonnee("ramzi", "par la queue");

	d7 = creerDonnee("yong", "souris");
	afficherDonnee(getHashTable("yong", table));

	uint64_t h = hash("yong") % table.taille;
	afficherListe(table.table_de_hachage[h]);

	printf("suppression d'un element pas dans la table\n");
	removeHashTable("yong", table);
	afficherDonnee(getHashTable("yong", table));

	putHashTable(d1, table);
	putHashTable(d2, table);
	putHashTable(d3, table);
	putHashTable(d4, table);
	putHashTable(d5, table);
	putHashTable(d6, table);

	afficherDonnee(getHashTable("yong", table));
	h = hash("ramzi") % table.taille;
	afficherLigneHashTable(table, h);
        afficherHashTable(table);
        
        
	printf("\n********affichage du tableu realloué 1111**********\n");
	reallocHashTable(&table,5,0);
	afficherHashTable(table);
	
	printf("\n********affichage du tableu realloué 222**********\n");
	reallocHashTable(&table,10,0);
	afficherHashTable(table);
}

