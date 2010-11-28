#include "hash.h"
#include <string.h>
#include<stdio.h>
#include<stdlib.h>








inline uint64_t hash(char* str) {

#ifdef DEBUG_MESSAGE
	printf("taille ds hash %d\n",TAILLE_HASH_TABLE);
#endif
	 uint64_t hash = 5381;
    
    assert(str!=NULL);  
#ifdef DEBUG_MESSAGE
	printf("la cle est: %s de longueur: %d \n",str,strlen(str));
#endif
	
    while(*str!='\0') {
        int c = *str;
        hash = ((hash << 5) + hash) + c;
        str ++;
    }
    return 5;
   // return (hash % TAILLE_HASH_TABLE);
}


liste_t* creerHashTable(uint64_t taille){

	assert(taille<=MAX_TAILLE_HASH_TABLE);
	TAILLE_HASH_TABLE= taille;
	tableDeHachage = (liste_t*)calloc(taille, sizeof(liste_t));
	if(tableDeHachage == NULL){
		perror("calloc()");
		exit(-1);
	}
#ifdef DEBUG_MESSAGE
	printf("taille %d\n",TAILLE_HASH_TABLE);
#endif
	return tableDeHachage;
}


/** retourne null si la valeur associé à cle n'est pas dans la table **/
donnee_t getHashTable(char* cle){
	
	uint64_t h;
	liste_t liste;
	assert(cle!=NULL);
	h=hash(cle);
	liste=tableDeHachage[h];
	return getKey(liste,cle);
}


void putHashTable(donnee_t D){


	uint64_t h;
	liste_t* liste_ptr;
	
	assert(D!=NULL);
	h=hash(D->cle);
	liste_ptr=&tableDeHachage[h];
	ajouterDonnee(liste_ptr,D);
}


char* removeHashTable(char* cle){


	uint64_t h;
	liste_t* liste_ptr;
	assert(cle!=NULL);
	h=hash(cle);
	liste_ptr=&tableDeHachage[h];
	return removeKey(liste_ptr,cle);
}

/**			    0 , 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,12			**/
/** tabHachage{ **,**,**,**,**,**,**,**,**,**,**,**,**,**}		    **/
/** apres decoupage avec indice=3									**/
/** tabHachage_un   ={ **,**,**,**}		    						**/
/** tabHachage_deux ={**,**,**,**,**,**,**,**,**,**}				**/
void segmenterHashTable(uint64_t indice,liste_t* tableDeHachage_un,liste_t* tableDeHachage_deux){

	
	assert(indice<=TAILLE_HASH_TABLE);
	
	tableDeHachage_deux   = creerHashTable(TAILLE_HASH_TABLE-indice);
	tableDeHachage_un     = creerHashTable(indice);

	int i=0;
	for(i=0;i<indice ;i++){
		*tableDeHachage_un[i]=*tableDeHachage[i];
	}
	for(i=indice;i<TAILLE_HASH_TABLE ;i++){
		*tableDeHachage_deux[i]=*tableDeHachage[i];
	}
	
	free(tableDeHachage);
}
