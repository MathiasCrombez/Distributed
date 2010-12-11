
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#define MAX_CMD_TAILLE 1000000
#define MAX_NB_ARG 10
#define PROMPT ">"

/* a modifier lors d'un ajout de commande */
#define NB_CMD 4
static char *commands[NB_CMD] = { "help", "put", "get", "exit" };




typedef enum { HELP, PUT, GET, EXIT, ERROR,VIDE } COMMAND;




/*
 * Affichage de l'aide
 */
void help()
{
	printf("Commandes disponibles :\n");
	printf("\t1)help:affiche l'aide\n");
	printf("\t2)get:");
	printf("\t3)put:");
}


/*
 * Determine la commande tapee
 * token : chaine correspondant a la commande uniquement
 * cmd : emplacement ou disposer la commande eventuellement identifiee
 */
COMMAND get_command(char *token)
{
	int i=0;
	COMMAND cmd;
	
	if(token==NULL){
		return VIDE;
	}
	for(i=0;i<NB_CMD;i++){
		if(strcmp(token,commands[i])==0)
			break;
	}
	
	if (i < NB_CMD){	
		cmd = i;
	}else{
		cmd = ERROR;
	}
	
	return cmd;
}





 /*
  * Analyse une ligne tapee
  * args : emplacement ou stocker la structure des arguments
  * retour : la commande tapee
  */
COMMAND read_command(char*** args)
{
	char* ligne = (char*)malloc(sizeof(char)*MAX_CMD_TAILLE+1);
	if(ligne==NULL){
		perror("malloc()");
		exit(-1);
	}
	
	fgets(ligne,MAX_CMD_TAILLE+1,stdin);
	
	char * lf = strchr(ligne, '\n'); /* On cherche le caractere '\n'. */
        if (lf != NULL) /* S'il est present, ... */
            *lf = '\0'; /* ... on le supprime    */
	//ligne[strlen(ligne)-1]=0;
	
	*args= (char**) malloc(sizeof(char*)*MAX_NB_ARG);
	if(*args==NULL){
		perror("malloc()");
		free(ligne);
		exit(-1);
	}
	
	int i=0;
	char* argument;
	char* iterateur_ligne = ligne;
	while(i<MAX_NB_ARG && (argument=strtok(iterateur_ligne," "))!=NULL){
		(*args)[i]=(char*)malloc(strlen(argument)*sizeof(char));
		strcpy(*(*args+i),argument);
		iterateur_ligne+=strlen(argument)+1;
		i++;
	}
	free(ligne);
	
	printf("command %s\n",*args [0]);
	
	return get_command(*args [0]);
}


int main()
{

	COMMAND cmd;
	char** args ;
	

	while (1) {
	#ifdef DEBUG
		printf("main: debut de la boucle de l'interpreteur\n");	
	#endif

		printf(PROMPT);

		cmd = read_command(&args);
		switch (cmd) {

		
		case HELP:
			help();
			break;

		case EXIT:
			goto end;

		case ERROR:

			printf("Commande incorrecte\n");
			break;
		}

	}
 end:	return 0;
}

