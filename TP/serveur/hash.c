#include "hash.h"





inline uint64_t hash(cle_t K)
{
	uint64_t hash = 5381;
	assert(K != NULL);
	while (*K != '\0') {
		int c = *K;
		hash = ((hash << 5) + hash) + c;
		K++;
	}
	return hash %MAX_TAILLE_HASH_TABLE;
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

	int i = 0;
	for (i = 0; i < hashTab.taille; i++) {
		libererListe(hashTab.table_de_hachage[i]);
	}
	/** le coup de grace ... **/
	hashTab.taille=0;
	free(hashTab.table_de_hachage);
}

/** retourne null si la valeur associé à cle n'est pas dans la table **/
donnee_t getHashTable(cle_t cle, table_de_hachage_t hashTab)
{
	uint64_t h;
	liste_t liste;
	assert(cle != NULL);
	assert(hashTab.taille != 0);
	assert(hashTab.table_de_hachage != NULL);
	h = hash(cle) % hashTab.taille;
	liste = hashTab.table_de_hachage[h];
	return getKey(liste, cle);
}

void putHashTable(donnee_t D, table_de_hachage_t hashTab)
{
	uint64_t h;
	liste_t *liste_ptr;
	assert(D != NULL);
	assert(hashTab.taille != 0);
	assert(hashTab.table_de_hachage != NULL);
	h = hash(D->cle) % hashTab.taille;
	liste_ptr = &hashTab.table_de_hachage[h];
	ajouterDonnee(liste_ptr, D);
}

valeur_t removeHashTable(cle_t cle, table_de_hachage_t hashTab)
{
	uint64_t h;
	liste_t *liste_ptr;
	assert(cle != NULL);
	assert(hashTab.taille != 0);
	assert(hashTab.table_de_hachage != NULL);
	h = hash(cle) % hashTab.taille;
	liste_ptr = &hashTab.table_de_hachage[h];
	return removeKey(liste_ptr, cle);
}

void afficherLigneHashTable(table_de_hachage_t hashTab, uint64_t numeroLigne)
{

	if (numeroLigne > hashTab.taille) {
#ifdef DEBUG_MESSAGE
		printf("afficherLigneHashTable:Ligne>Taille(%ld)",(long)hashTab.taille);
#endif
	} else {
		afficherListe(hashTab.table_de_hachage[numeroLigne]);
	}
}

void afficherHashTable(table_de_hachage_t hashTab)
{
	int i = 0;

	printf("afficherHashTable:Debut\n");
	printf("\tHashTab size= %d\n",hashTab.taille);
	for (i = 0; i < hashTab.taille; i++) {
		printf("afficherHashTable:Ligne %d\n", i);
		afficherLigneHashTable(hashTab, i);
	}
	printf("afficherHashTable:Fin\n");
}

//creer une table de hachage juste pour les testes
table_de_hachage_t TEST_HASH_TABLE()
{

    table_de_hachage_t tablh = creerHashTable(MAX_TAILLE_HASH_TABLE);

	donnee_t d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;
	donnee_t d11, d12, d13, d14, d15, d16, d17, d18, d19, d20;

	assert(NULL != (d1 = creerDonnee("a", "AAAAAAAAAAA")));
	assert(NULL != (d2 = creerDonnee("b", "BBBBBBBBBBB")));
	assert(NULL != (d3 = creerDonnee("c", "CCCCCCCCCCC")));
	assert(NULL != (d4 = creerDonnee("d", "DDDDDDDDDDD")));
	assert(NULL != (d5 = creerDonnee("e", "EEEEEEEEEEE")));
	assert(NULL != (d6 = creerDonnee("f", "FFFFFFFFFFF")));
	assert(NULL != (d7 = creerDonnee("g", "GGGGGGGGGGG")));
	assert(NULL != (d8 = creerDonnee("h", "HHHHHHHHHHH")));
	assert(NULL != (d9 = creerDonnee("i", "IIIIIIIIIII")));
	assert(NULL != (d10 = creerDonnee("j", "JJJJJJJJJJJ")));
	assert(NULL != (d11 = creerDonnee("k", "KKKKKKKKKKK")));
	assert(NULL != (d12 = creerDonnee("l", "LLLLLLLLLLL")));
	assert(NULL != (d13 = creerDonnee("m", "MMMMMMMMMMM")));
	assert(NULL != (d14 = creerDonnee("n", "NNNNNNNNNNN")));
	assert(NULL != (d15 = creerDonnee("o", "OOOOOOOOOOO")));
	assert(NULL != (d16 = creerDonnee("p", "PPPPPPPPPPP")));
	assert(NULL != (d17 = creerDonnee("q", "QQQQQQQQQQQ")));
	assert(NULL != (d18 = creerDonnee("r", "RRRRRRRRRRR")));
	assert(NULL != (d19 = creerDonnee("s", "SSSSSSSSSSS")));
	assert(NULL != (d20 = creerDonnee("t", "TTTTTTTTTTT")));

	putHashTable(d1, tablh);
	putHashTable(d2, tablh);
	putHashTable(d3, tablh);
	putHashTable(d4, tablh);
	putHashTable(d5, tablh);
	putHashTable(d6, tablh);
	putHashTable(d7, tablh);
	putHashTable(d8, tablh);
	putHashTable(d9, tablh);
	putHashTable(d10, tablh);
	putHashTable(d11, tablh);
	putHashTable(d12, tablh);
	putHashTable(d13, tablh);
	putHashTable(d14, tablh);
	putHashTable(d15, tablh);
	putHashTable(d16, tablh);
	putHashTable(d17, tablh);
	putHashTable(d18, tablh);
	putHashTable(d19, tablh);
	putHashTable(d20, tablh);

	return tablh;
}



void reallocHashTable(table_de_hachage_t* hashTab,uint32_t new_size, uint64_t h, pthread_mutex_t ** mutexTab)
{
        assert(hashTab!=NULL);
        liste_t* new_hash_tab;
        int i;

        if(new_size<=hashTab->taille){
                assert(new_size>0);
                new_hash_tab= (liste_t*)realloc(hashTab->table_de_hachage,new_size*sizeof(liste_t));
                if(new_hash_tab==NULL){
                
                        perror("realloc()");
                        exit(-1);
                }
                
               
        }
        else {
                assert(new_size<=MAX_TAILLE_HASH_TABLE);
                new_hash_tab = (liste_t* )calloc(new_size, sizeof(liste_t));
                if(new_hash_tab==NULL){
                
                        perror("realloc()");
                        exit(-1);
                }
                memcpy(new_hash_tab +h%new_size,hashTab->table_de_hachage,sizeof(liste_t)*hashTab->taille);
                free(hashTab->table_de_hachage);
                
        }
        hashTab->table_de_hachage= new_hash_tab;
        hashTab->taille = new_size;
        free(*mutexTab);
        *mutexTab = malloc(new_size * sizeof(pthread_mutex_t));
        for(i = 0; i < new_size; i++) {
                pthread_mutex_init((*mutexTab)[i], NULL);
        }
}
