#include "commun.h"
#include "client_impl.h"
#include "message.h"


#define MAX_CMD_TAILLE 100
#define MAX_NB_ARG 10
#define PROMPT ">"

/* a modifier lors d'un ajout de commande */
#define NB_CMD 9
static char *commands[NB_CMD] = { 
    "help", 
    "connect", 
    "disconnect", 
    "put", 
    "get", 
    "removekey",
    "quit" ,
    "status",
    "exit",
    
};


typedef enum { 
    _HELP, 
    _CONNECT,
    _DISCONNECT,
    _PUT, 
    _GET, 
    _REMOVEKEY, 
    _QUIT, 
    _STATUS,
    _EXIT, 
    _ERROR,
    _VIDE,
    
}COMMAND;




/*
 * Affichage de l'aide
 */
void help()
{
    printf("Commandes disponibles :\n");
    printf("\thelp:affiche l'aide\n");
    printf("\tput:\n");
    printf("\tget:\n");
    printf("\tremovekey:\n");
    printf("\tconnect [ip du serveur] [port]\n");
    printf("\tquit \n");
    printf("\tstatus \n");
}


/*
 * Determine la commande tapee
 * token : chaine correspondant a la commande uniquement
 * cmd : emplacement ou disposer la commande eventuellement identifiee
 */
inline COMMAND get_command(char *token)
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
	
   // printf("command %s\n",*args [0]);
	
    return get_command(*args [0]);
}



//==============================================================================
//                              INTERPRETEUR
//==============================================================================




void interpreteur()
{

    COMMAND cmd;
    char** args ;
    donnee_t D;
    valeur_t V;
    socket_t sockServer;
    int IS_CONNECTED=0;
    char name[20];
    idConnexion_t ident;
    
    /* creation d'un client */
    printf("Entrez le nom du client (20 car max!)\n");
    fgets(name,19,stdin);
    creerClient(name);
    printf("Creation client ok\n");
    
    while (1) {
#ifdef DEBUG
        printf("main: debut de la boucle de l'interpreteur\n");	
#endif

        printf(PROMPT);

        cmd = read_command(&args);
        switch (cmd) {
        
        
        case _CONNECT:
        
            if(IS_CONNECTED==0){
            
                printf("_CONNECT %s %s\n", args[1], args[2]);
                sockServer = message_connect(args[1], atoi(args[2]));
                IS_CONNECTED=1;
            } else {
                
                printf("Vous êtes deja connecté à un serveur.\n");
            }
            break;
        
            
        case _PUT:
        
            if(IS_CONNECTED==0){
            
                printf("Impossible vous êtes déconnecté!\n");
                break;
            }
            D = creerDonnee(args[1],args[2]);
            message_put(D,sockServer);
            afficherDonnee(D);
            libererDonnee(D);
            break;
        
            
        case _GET:
        
            if(IS_CONNECTED==0){
            
                printf("Impossible vous êtes déconnecté!\n");
                break;
            }
            D= message_get(args[1],sockServer);
            afficherDonnee(D);
            libererDonnee(D);                
            break;
            
        case _REMOVEKEY:
        
            if(IS_CONNECTED==0){
             
                printf("Impossible vous êtes déconnecté!\n");
                break;
            }
            
            V = message_remove(args[1],sockServer);
            printf("Valeur supprimee:%s\n", V);
            free(V);
            break;
            
        case _DISCONNECT:
        
            if(IS_CONNECTED==1){
            
                printf("deconnexion du client\n");
                message_disconnect(sockServer);
                printf("deconnexion réussie\n");
                IS_CONNECTED=0;
            } else {
            
                printf("le client est deja deconnecté\n");
            }
            break;
            
        case _HELP:
        
            help();
            break;
            
        case _EXIT:
        
            break;
        case _ERROR:

            printf("Commande incorrecte\n");
            break;
            
        case _QUIT:
        
            if(IS_CONNECTED==1){
            
                printf("fermeture du serveur\n");
                message_quit(sockServer);
                printf("deconnexion réussie\n");
                IS_CONNECTED=0;
            } else {
            
                printf("le client est deconnecté\n");
            }
            
            break;
                
        case _STATUS:
        
            printf("status\n");
            
            if(IS_CONNECTED==0){
             
                printf("Impossible vous êtes déconnecté!\n");
                break;
            }
            
            ident=message_status(sockServer);
            afficherIdentConnexion(ident);
        default:
         //   printf("Cas Impossible\n");
            break;
        }


    }
    // end:	return 0;
}

