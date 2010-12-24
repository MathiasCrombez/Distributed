#include"serveur_impl.h"
/**
 *Implémente les messages que s'envoient les serveurs entres eux seulement
 **/




//==============================================================================
//                        FO?CTIONS INLINES
//==============================================================================



/*
 * Connect à un server.Renvoi le socket qui permet de communiquer avec le client
 */
inline socket_t ___connect2server___(struct sockaddr_in server_info)
{
        socket_t idSocket;

        idSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (idSocket < 0) {
                perror("socket()");
                exit(-1);
        }

        if (connect(idSocket, (struct sockaddr *)&server_info,sizeof(struct sockaddr_in)) == -1) {
                perror("connect()");
                exit(-1);
        }
        return idSocket;
}


/*
 * renvoi une structure sockaddr_in avec l'ip et le port de connexion
 * protocole TCP/IP
 */
inline struct sockaddr_in ___get_sockaddr_in___(char* ip , uint32_t port)
{

	struct hostent *hostinfo ;
	struct sockaddr_in serv_addr;
	
	hostinfo = gethostbyname(ip);
	if (hostinfo == NULL) {
		printf("___get_sockaddr_in__:Erreur gethostbyname()\n",ip);
		exit(-1);
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port   = htons(port);
	serv_addr.sin_addr   = *(struct in_addr *)hostinfo->h_addr;

	return serv_addr;
}


/*
* renvoi les infos de connexion du prochain serveur
*/
inline struct sockaddr_in ___message_whois_next_server___(struct sockaddr_in serv_addr)
{
 
 	struct sockaddr_in next_server_info;
 	socket_t sock_next_server;
 	
 	sock_next_server=___connect2server___(serv_addr);
	
		
 	/* communication avec le serveur */
 	envoyerOrigine(FROM_SERVEUR,sock_next_server);
 	envoyerTypeMessage(WHOIS_NEXT_SERVER,sock_next_server);
 	
 	recevoirSockAddr(&next_server_info, sock_next_server);
 	close(sock_next_server);
 	return next_server_info;
 }


/*
 * renvoi les infos de connexion du serveur décrit par la structure serv_addr
 */
inline idConnexion_t ___message_ident___(struct sockaddr_in serv_addr)
{

        idConnexion_t server_info;
        socket_t sock_server;

        sock_server=___connect2server___(serv_addr);
     
        envoyerOrigine(FROM_SERVEUR,sock_server);
        envoyerTypeMessage(IDENT,sock_server);

        recevoirIdent(&server_info, sock_server);
        close(sock_server);
        return server_info;
}


/*
 * se connecte au serveur indiqué par server_info
 */
inline int ___message_connect_to___(struct sockaddr_in server_info)
{

	socket_t new_socket;
	socket_t sockServer;
	char reponse=0;
	idConnexion_t id_connexion;
	serveur_t* my_server_ptr;
	
	my_server_ptr= get_my_server();
	
	/* 
	 * Connexion au serveur dont les identifiants sont dans server_info
	 * recuperation des identifiants du serveur suivant dans le cercle
	 */
	sockServer = ___connect2server___(server_info);
	
	
	envoyerOrigine(FROM_SERVEUR,sockServer);
	envoyerTypeMessage(CONNECT,sockServer);
	
	recevoirSockAddr(&(my_server_ptr->suivServeur), sockServer);
	envoyerSockAddr(my_server_ptr->serv_addr,sockServer);
	/*
	 *on libere la connexion 
	 */
	if (close(sockServer)<0) {
        	printf("___message_connect_to__:Echec de la premiere deconnexion du serveur\n");
         	exit(-1);
        }
	
#ifdef DEBUG_MESSAGE_SERVEUR
	
	printf("___message_connect_to__:Serveur suivant:\n");
	afficherIdentConnexion(my_server_ptr->suivServeur);
	printf("\n");
#endif
        return 1;
}


/*
 * transfere la dht depuis from_server vers le serveur actuel.
 * La serveur actuel va s'occuper de la table de taille taille est des donnés
 * de hash>h
 * message= transfere moi une partie de ta table de hachage
 */
inline int ___message_receive_DHT_from___(idConnexion_t from_server, uint64_t h)
{

        table_de_hachage_t hashTab;
        uint32_t taille;
        char reponse;
        socket_t sock_server;
        liste_t L;
        donnee_t D;
        serveur_t* my_server_ptr = get_my_server();
        int i;
        /*
         * connexion au serveur
         */
        sock_server=___connect2server___(from_server.identifiant);
        envoyerOrigine(FROM_SERVEUR,sock_server);
        envoyerTypeMessage(RECEIVE_DHT,sock_server);
        /*
         * Construction de la nouvelle table de hachage
         */
        printf("connexion et reception de la table\n");
        taille = from_server.taille_hashtab/2;
        assert(taille>0);
        
        hashTab = creerHashTable(taille);
        
        envoyerHash(h,sock_server);
        
        my_server_ptr->tabl.mutexTab = malloc(taille * sizeof(pthread_mutex_t));
        for(i = 0; i< taille; i++) {
                pthread_mutex_init(&(my_server_ptr->tabl.mutexTab[i]), NULL);
        }
        
        
        recevoirOctet(&reponse,sock_server);
        
        while(reponse){
        
                recevoirDonnee(&D,sock_server);
                putHashTable(D,hashTab);
                recevoirOctet(&reponse,sock_server);
        }
        
        my_server_ptr->h=h;
        libererHashTable(my_server_ptr->tabl);
        my_server_ptr->tabl= hashTab;

        afficherInfoHashTable();
        if (close(sock_server)<0) {
        	printf("___message_connect_to__:Echec de la premiere deconnexion du serveur\n");
         	exit(-1);
        }
}


int ____message_transfer_DHT_to_next_server____(){


        socket_t sock_server;
        uint32_t taille_hashtab;
        liste_t L;
        int i;
        uint64_t h;
        donnee_t D;
        struct sockaddr_in to_server; 
        serveur_t* my_server_ptr = get_my_server();
        to_server = my_server_ptr->suivServeur;
        /*
         * connexion au serveur
         */
        sock_server=___connect2server___(to_server);
        envoyerOrigine(FROM_SERVEUR,sock_server);
        envoyerTypeMessage(QUIT,sock_server);
        
        table_de_hachage_t my_hashtab = get_my_hashtab();
        taille_hashtab = my_hashtab.taille;
        h= my_server_ptr->h;
        
        envoyerUInt_32(taille_hashtab,sock_server);
        envoyerUInt_64(h,sock_server);
        
        
        for(i=0;i<taille_hashtab;i++){
                
                L=my_hashtab.table_de_hachage[i];
                        
                while(L!=NULL){
                        D=removeTeteDeListe(&L);
                        assert(D!=NULL);//aucune donné ne devrait etre nul
                        envoyerOctet(1,sock_server);
                        envoyerDonnee(D,sock_server);
                        afficherDonnee(D);
                        libererDonnee(D);
                }
        }
        envoyerOctet(0,sock_server);
        free(my_hashtab.table_de_hachage);
        my_hashtab.taille = 0;
        //prevenir le serveur qui est avant dans le cercle
        
}
//==============================================================================
//                        FONCTIONS DE COMMUNICATION SERVEUR-SERVEUR
//==============================================================================


/*
 * Connection au serveur d'adresse IP 'ip' sur le port 'port'.
 * Equilibrage de charge:Apres un parcour de la dht, on se connecte au
 * serveur le plus chargé
 */
int message_connect_2_server(char* ip,uint32_t port){

        uint64_t taille_max;
        struct sockaddr_in serv_addr;
        idConnexion_t iterator_server_info;
        idConnexion_t server_most_charged;
        uint64_t h;

        serv_addr = ___get_sockaddr_in___(ip,port);

        //itinialisation

        taille_max = 0;
        iterator_server_info = ___message_ident___(serv_addr);
        
        
        while(1){

                iterator_server_info=___message_ident___(serv_addr);
                
                if(iterator_server_info.taille_hashtab > taille_max){
                        taille_max = iterator_server_info.taille_hashtab ;
                        server_most_charged=iterator_server_info;
                }
                
                serv_addr=___message_whois_next_server___(serv_addr);
                
                if( serv_addr.sin_port==htons(port) ) {

                        /*
                        * on a fait le tour des serveurs.Il reste plus qu'à se connecter au serveur le plus chargé
                        */
                        h=(server_most_charged.h+taille_max/2);
                        printf("le serveur le plus chargé\n");
                        afficherIdentConnexion(server_most_charged);
                        ___message_connect_to___(server_most_charged.identifiant);
                        ___message_receive_DHT_from___(server_most_charged,h);
                        afficherHashTable(get_my_hashtab());
                        return 1;
                }
        }
}




int message_whois_next_server(char* ip, uint32_t port)
{

        struct sockaddr_in serv_addr;
        struct sockaddr_in server_info;

        serv_addr=___get_sockaddr_in___(ip,port);
        server_info=___message_whois_next_server___(serv_addr);
#ifdef DEBUG_MESSAGE_SERVEUR
#endif
        return 1;
}


int message_ident(char* ip, uint32_t port)
{

        struct sockaddr_in serv_addr;
        idConnexion_t server_info;

        serv_addr=___get_sockaddr_in___(ip,port);
        server_info=___message_ident___(serv_addr);
#ifdef DEBUG_MESSAGE_SERVEUR
        afficherIdentConnexion(server_info);
#endif
        return 1;
}


int message_quit()
{
        
        struct sockaddr_in serv_addr;
        serveur_t* my_server_ptr;
        my_server_ptr = get_my_server();
        
        if(my_server_ptr->serv_addr.sin_port!=my_server_ptr->suivServeur.sin_port) {
        
                ____message_transfer_DHT_to_next_server____();
        } else { 
                //TODO SYNCHRO ATTENDRE QUE LES REQUETES SOIENT TT SATISFAITES
                exit(0);//TODO TUE LE TABLEAU
        }
        return 1 ;
}





