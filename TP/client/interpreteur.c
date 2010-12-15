
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../commun/commun.h"
#include "client_impl.h"
#include "message.h"


#define MAX_CMD_TAILLE 1000000
#define MAX_NB_ARG 10
#define PROMPT ">"

/* a modifier lors d'un ajout de commande */
#define NB_CMD 6
static char *commands[NB_CMD] = { "help", "connect", "put", "get", "removekey","exit" };




typedef enum { _HELP, _CONNECT, _PUT, _GET, _REMOVEKEY, _EXIT, _ERROR,_VIDE } COMMAND;




/*
 * Affichage de l'aide
 */
void help()
{
    printf("Commandes disponibles :\n");
    printf("\thelp:affiche l'aide\n");
    printf("\tput:");
    printf("\tget:");
    printf("\tremovekey:");
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
        return _VIDE;
    }
    for(i=0;i<NB_CMD;i++){
        if(strcmp(token,commands[i])==0)
            break;
    }
	
    if (i < NB_CMD){	
        cmd = i;
    }else{
        cmd = _ERROR;
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
    donnee_t D;
    client_t* client_ptr = NULL;
    socket_t sockServer;

    while (1) {
#ifdef DEBUG
        printf("main: debut de la boucle de l'interpreteur\n");	
#endif

        printf(PROMPT);

        cmd = read_command(&args);
        switch (cmd) {
        case _CONNECT:
            printf("_CONNECT %s %s %s\n", args[1], args[2], args[3]);
            //            if (client_ptr) {
                client_ptr = creerClient(args[1]);
                sockServer = message_connect(preConnect(args[2], atoi(args[3])));
                /*            }
            else {
            }*/
            break;
        case _PUT:
            D = creerDonnee(args[1],args[2]);
            message_put(D,sockServer);
            break;
        case _GET:
            afficherDonnee(message_get(args[1],sockServer));                
            break;
        case _REMOVEKEY:
            printf("Valeur supprimee:%s\n", message_remove(args[1],sockServer));
            break;		
        case _HELP:
            help();
            break;

        case _EXIT:
            message_disconnect(sockServer);
            //      goto end;
            exit(0);
            break;
        case _ERROR:

            printf("Commande incorrecte\n");
            break;
        default:
            printf("Cas Impossible\n");
            break;
        }


    }
    // end:	return 0;
}

