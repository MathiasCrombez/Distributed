#include "hash.h"

inline uint64_t hash(cle_t K)
{
	uint64_t hash = 5381;
	assert(K != NULL);
#ifdef DEBUG_MESSAGE
	printf("la cle est: %s de longueur: %d \n", K, strlen(K));
#endif

	while (*K != '\0') {
		int c = *K;
		hash = ((hash << 5) + hash) + c;
		K++;
	}
	//return 5;
	return hash;
}

table_de_hachage_t creerHashTable(uint64_t taille)
{
	table_de_hachage_t hashTab;

	assert(taille <= MAX_TAILLE_HASH_TABLE);
	hashTab.taille = taille;
	hashTab.table_de_hachage = (liste_t *) calloc(taille, sizeof(liste_t));
	if (hashTab.table_de_hachage == NULL) {
		perror("calloc()");
		exit(-1);
	}

	return hashTab;
}

void libererHashTable(table_de_hachage_t hashTab)
{
	if (hashTab.table_de_hachage == NULL) {
#ifdef DEBUG_MESSAGE
		printf("libererHashTable: la table de hachage est deja vide\n");
#endif
		return;
	} else {

		int i = 0;
		for (i = 0; i < hashTab.taille; i++) {
			libererListe(hashTab.table_de_hachage + i);
		}
		/** le coup de grace ... **/
		hashTab.taille = 0;
		free(hashTab.table_de_hachage);
	}
}

/** retourne null si la valeur associé à cle n'est pas dans la table **/
donnee_t getHashTable(char *cle, table_de_hachage_t hashTab)
{
	uint64_t h;
	liste_t liste;
	assert(cle != NULL);
	h = hash(cle) % hashTab.taille;
	liste = hashTab.table_de_hachage[h];
	return getKey(liste, cle);
}

void putHashTable(donnee_t D, table_de_hachage_t hashTab)
{
	uint64_t h;
	liste_t *liste_ptr;

	assert(D != NULL);
	h = hash(D->cle) % hashTab.taille;
	liste_ptr = &hashTab.table_de_hachage[h];
	ajouterDonnee(liste_ptr, D);
}

valeur_t removeHashTable(cle_t cle, table_de_hachage_t hashTab)
{
	uint64_t h;
	liste_t *liste_ptr;
	assert(cle != NULL);
	h = hash(cle) % hashTab.taille;
	liste_ptr = &hashTab.table_de_hachage[h];
	return removeKey(liste_ptr, cle);
}

void afficherLigneHashTable(table_de_hachage_t hashTab, uint64_t numeroLigne)
{

	if (numeroLigne > hashTab.taille) {
#ifdef DEBUG_MESSAGE
		printf("afficherLigneHashTable: indice > %d", hashTab.taille);
#endif
		return;
	} else {
		afficherListe(hashTab.table_de_hachage[numeroLigne]);
	}
}

void segmenterHashTable(uint64_t indice,
			table_de_hachage_t * hash_tab1_ptr,
			table_de_hachage_t * hash_tab2_ptr)
{
/*	assert(indice <= TAILLE_HASH_TABLE);*/

/*	*hash_tab1_ptr = creerHashTable(indice);*/
/*	*hash_tab2_ptr = creerHashTable(TAILLE_HASH_TABLE - indice);*/

/*	int i = 0;*/
/*	for (i = 0; i < indice; i++) {*/
/*		hash_tab1_ptr->table_de_hachage[i] = TABLE_DE_HACHAGE[i];*/
/*	}*/
/*	for (i = indice; i < TAILLE_HASH_TABLE; i++) {*/
/*		hash_tab2_ptr->table_de_hachage[i] = TABLE_DE_HACHAGE[i];*/
/*	}*/

/*	hash_tab2_ptr->taille = TAILLE_HASH_TABLE - indice;*/
/*	hash_tab1_ptr->taille = indice;*/
/*	free(TABLE_DE_HACHAGE);*/
}

