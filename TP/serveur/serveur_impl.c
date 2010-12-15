#include "serveur_impl.h"

#define SET_SERVEUR_NAME(NAME,PORT){                                \
        strcpy(SERVEUR.name,NAME);                                \
        strcat(SERVEUR.name,":");                                \
        sprintf(SERVEUR.name+strlen(NAME)+1,"%llu",PORT);        \
}


/*
 *Initialisation de la variable globale SERVEUR.
 */
serveur_t* creerServeur(char *nomDuServeur, uint64_t port)
{

        int yes = 1;

        SET_SERVEUR_NAME(nomDuServeur, port);
        SERVEUR.idSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (SERVEUR.idSocket < 0) {
            perror("socket()");
            exit(EXIT_FAILURE);
        }
        
        //initialisation de la structure serveur
        SERVEUR.serv_addr.sin_family = AF_INET;
        SERVEUR.serv_addr.sin_addr.s_addr = inet_addr(SERVEURNAME);
        SERVEUR.serv_addr.sin_port = htons(port);

        
        SERVEUR.suivServeur = setIdConnexion(SERVEUR.name,SERVEUR.serv_addr,0,0);

        // evite le message d'erreur "Address already in use" lors d'un bind
        if (setsockopt (SERVEUR.idSocket, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
                perror("setsockopt()");
                exit(EXIT_FAILURE);
        }

        if (bind(SERVEUR.idSocket, (struct sockaddr *)&SERVEUR.serv_addr, sizeof(SERVEUR.serv_addr)) < 0) {
                perror("bind()");
                exit(EXIT_FAILURE);
        }

        if (listen(SERVEUR.idSocket, LENGTH_LISTEN_QUEUE)) {
                perror("liste()");
                exit(EXIT_FAILURE);
        }
        return &SERVEUR;
}

/**
 *A chaque connection d'un client, un thread est créé et talk_to_client est appelée.
 */
void *talk_to_client(void *idSocket)
{
    cle_t K;
    donnee_t D;
    requete_t type_requete;
    socket_t sockClient = *(socket_t*) idSocket;
    tabClient_t curr, prev;
    uint64_t h;
    while(1) {
        recevoirTypeMessage(&type_requete, sockClient);
        switch (type_requete) {
            
        case PUT:
            recevoirCle(&K,sockClient);

            h = hash(K) % MAX_TAILLE_HASH_TABLE;
            printf("talk_to_client:PUT:cle(%s), hash(%llu)\n",K, h);
            if ( ! (SERVEUR.h <= h && h <= (SERVEUR.h + SERVEUR.tabl.taille))){
                envoyerOctet(0,sockClient);
                envoyerIdent(SERVEUR.suivServeur, sockClient);
            } else {
                envoyerOctet(1,sockClient);
                recevoirDonnee(&D,sockClient);
                putHashTable(D,SERVEUR.tabl);
            }
            break;

        case GET:
            recevoirCle(&K,sockClient);

            h = hash(K) % MAX_TAILLE_HASH_TABLE;
            printf("talk_to_client:GET:cle(%s), hash(%llu)\n",K, h);
            if ( ! (SERVEUR.h <= h && h <= (SERVEUR.h + SERVEUR.tabl.taille))){
                envoyerOctet(0,sockClient);
                envoyerIdent(SERVEUR.suivServeur, sockClient);
            } else {
                D = getHashTable(K,SERVEUR.tabl); 
                envoyerOctet(1,sockClient);
                if (D==NULL) {
                    envoyerOctet(0,sockClient);
                } else {
                    envoyerOctet(1,sockClient);
                    envoyerDonnee(D,sockClient);
                }
            }
            break;

        case REMOVEKEY:
            recevoirCle(&K,sockClient);

            h = hash(K) % MAX_TAILLE_HASH_TABLE;
            printf("talk_to_client:cle(%s), hash(%llu)\n",K, h);
            if ( ! (SERVEUR.h <= h && h <= (SERVEUR.h + SERVEUR.tabl.taille))){
                envoyerOctet(0,sockClient);
                envoyerIdent(SERVEUR.suivServeur, sockClient);
            } else {
                D = getHashTable(K,SERVEUR.tabl); 
                envoyerOctet(1,sockClient);
                if (D==NULL) {
                    envoyerOctet(0,sockClient);
                } else {
                    valeur_t V = removeHashTable(K,SERVEUR.tabl);
                    envoyerOctet(1,sockClient);
                    envoyerValeur(V, sockClient);
                    free(V);
                }
            }
            break;
        case IDENT:
            break;

        case CONNECT:
            envoyerOctet(1,sockClient);
            break;
            
        case DISCONNECT:
            curr = SERVEUR.tableauClient;
            prev = SERVEUR.tableauClient;
            if (curr->client.idSocket == sockClient) {
                SERVEUR.tableauClient = curr->suiv;
                free(curr);
            }
            else {
                while(curr->client.idSocket != sockClient && curr) {
                    prev = curr;
                    curr = curr->suiv;                    
                }
                if (curr) {
                    prev->suiv = curr->suiv;
                    free(curr);
                }
                else {
                    printf("talk_to_client:DISCONNECT:client de socket %d inconnu.\n"
                           , sockClient);
                    close(sockClient);
                    pthread_exit(NULL);
                }
            }
            close(sockClient);
            pthread_exit(NULL);
            break;
        default:
            break;

        }
    }
}
/**
* Corps de la fonction de routine lors de la création de pthread
* (apparition d'un client
*/
void *talk_to_server(void *idSocket)
{
        //ce socket va nous permettre de communiquer avec le client
        socket_t sockServer = *(socket_t*)idSocket;
        char reponse;
        char *nom;
        idConnexion_t id_connexion;
        requete_t type_requete;
        donnee_t D;
        liste_t L;
        uint64_t h;
        table_de_hachage_t my_hashtab;
        uint32_t taille;
#ifdef DEBUG_SERVEUR_IMPL
        printf("########debut du thread#########\n");
#endif
        
        recevoirTypeMessage(&type_requete, sockServer);
        switch (type_requete) {
        

        case CONNECT:
        
                printf("SERVER CONNECT\n");
                id_connexion = get_my_idConnexion();
                envoyerIdent(SERVEUR.suivServeur, sockServer);
                recevoirIdent(&SERVEUR.suivServeur,sockServer);

        #ifdef DEBUG_SERVEUR_IMPL

                printf("****serveur suivant est:****\n");
                afficherIdentConnexion(SERVEUR.suivServeur);
                printf("\n");
        #endif
                
                break;

        case IDENT:
        
                printf("IDENT\n");
                id_connexion= get_my_idConnexion();
                afficherIdentConnexion(id_connexion);
                envoyerIdent(id_connexion,sockServer);
                break;
                
                
        case WHOIS_NEXT_SERVER:
        
                printf("WHOIS_NEXT_SERVER\n");
                envoyerIdent(SERVEUR.suivServeur,sockServer);
                break;
        
        
        /* un serveur demande de partager la table de hachage*/
        case RECEIVE_DHT:
        
                printf("TRANSFER DHT\n");
                my_hashtab = get_my_hashtab();
                liste_t L;
                uint64_t i;
                donnee_t D;
                recevoirHash(&h,sockServer);                  
                
                for(i=h-SERVEUR.h;i<my_hashtab.taille;i++){
                
                        L=my_hashtab.table_de_hachage[i];
                        
                        while(L!=NULL){
                                D=removeTeteDeListe(&L);
                                assert(D!=NULL);//aucune donné ne devrait etre nul
                                envoyerOctet(1,sockServer);
                                envoyerDonnee(D,sockServer);
                                libererDonnee(D);
                        }
                }
        //fin d'envoi
                envoyerOctet(0,sockServer);
                     
                reallocHashTable(&SERVEUR.tabl,SERVEUR.tabl.taille/2);
                SERVEUR.suivServeur.h=h;
        
                afficherInfoHashTable();
                afficherHashTable(SERVEUR.tabl);
                break;
/*        case DISCONNECT:*/

/*                printf("un serveur souhaite ma mort\n");*/
/*                recevoirIdent(&(SERVEUR.precServeur->identifiant), sockServer);*/
/*                recevoirChaine(&(SERVEUR.precServeur->name), sockServer);*/
/*                printf("c'est fait!\n");*/
/*                return NULL;*/
        default:
                printf("message incinnu");
                break;

        }
        
        /* fermeture de la communication et mort ddu thread*/
        
        printf("########fin du thread#########\n");
        shutdown(sockServer, SHUT_RDWR);
        pthread_exit(NULL);

}




idConnexion_t get_my_idConnexion()
{
        return setIdConnexion( SERVEUR.name,
                                  SERVEUR.serv_addr,
                                  SERVEUR.h,
                                  SERVEUR.tabl.taille
                                 );
}


serveur_t* get_my_server()
{
        return &SERVEUR;
}

table_de_hachage_t get_my_hashtab()
{
        return SERVEUR.tabl;
}

/*
 * methode de debug
 */
 
void afficherInfoHashTable(){

        printf("Info sur hash table\n");
        
        printf("****\n\tma taille: %d\n",SERVEUR.tabl.taille);
        printf("\tmon h: %d\n",SERVEUR.h);
        printf("\t h de suiv: %d\n",SERVEUR.suivServeur.h);
}
