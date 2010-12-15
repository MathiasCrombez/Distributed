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
                //TODO RESSAYER DE CE CONNECTER PLUS TARD .
                //TODO NE PAS TERMINER PAR EXIT(-1) A LA PREMIERE TENTATIVE DE CONNEXION
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
inline idConnexion_t ___message_whois_next_server___(struct sockaddr_in serv_addr)
{
 
 	idConnexion_t next_server_info;
 	socket_t sock_next_server;
 	
 	sock_next_server=___connect2server___(serv_addr);
	
		
 	/* communication avec le serveur */
 	envoyerOrigine(FROM_SERVEUR,sock_next_server);
 	envoyerTypeMessage(WHOIS_NEXT_SERVER,sock_next_server);
 	
 	recevoirIdent(&next_server_info, sock_next_server);
 	return next_server_info;
 }


/*
 * renvoi les infos de connexion du serveur décrit par la structure serv_addr
 */
inline idConnexion_t ___message_ident___(struct sockaddr_in serv_addr)
{

        idConnexion_t server_info;
        socket_t sock_next_server;

        sock_next_server=___connect2server___(serv_addr);
     
        envoyerOrigine(FROM_SERVEUR,sock_next_server);
        envoyerTypeMessage(IDENT,sock_next_server);

        recevoirIdent(&server_info, sock_next_server);
        shutdown(sock_next_server,SHUT_RDWR);
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
	
	
	id_connexion= get_my_idConnexion();
	my_server_ptr= get_my_server();
	
	/* 
	 * Connexion au serveur dont les identifiants sont dans server_info
	 * recuperation des identifiants du serveur suivant dans le cercle
	 */
	sockServer = ___connect2server___(server_info);
	
	
	envoyerOrigine(FROM_SERVEUR,sockServer);
	envoyerTypeMessage(CONNECT,sockServer);
	
	recevoirIdent(&my_server_ptr->suivServeur, sockServer);
	envoyerIdent(id_connexion,sockServer);
	/*
	 *on libere la connexion 
	 */
	if (shutdown(sockServer,SHUT_RDWR)) {
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
        assert(taille!=0);
        
        hashTab = creerHashTable(taille);
        
        envoyerHash(h,sock_server);
        
        int i=0;
        
        
        recevoirOctet(&reponse,sock_server);
        
        while(reponse){
        
                recevoirDonnee(&D,sock_server);
                putHashTable(D,hashTab);
                recevoirOctet(&reponse,sock_server);
        }
        
        my_server_ptr->h=h;
        afficherIdentConnexion(my_server_ptr->suivServeur);
        libererHashTable(my_server_ptr->tabl);
        my_server_ptr->tabl= hashTab;
        afficherInfoHashTable();
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

                if(iterator_server_info.taille_hashtab > taille_max){
                        taille_max = iterator_server_info.taille_hashtab ;
                        server_most_charged=iterator_server_info;
                }
                
                iterator_server_info=___message_whois_next_server___(serv_addr);
                serv_addr = iterator_server_info.identifiant;
                if( serv_addr.sin_port==htons(port) ) {

                        /*
                        * on a fait le tour des serveurs.Il reste plus qu'à se connecter au serveur le plus chargé
                        */
             
                        h=(server_most_charged.h+taille_max/2);
                        afficherIdentConnexion(server_most_charged);
                        ___message_connect_to___(server_most_charged.identifiant);
                        ___message_receive_DHT_from___(server_most_charged,h);
                        return 1;
                }
        }
}




int message_whois_next_server(char* ip, uint32_t port){

        struct sockaddr_in serv_addr;
        idConnexion_t server_info;

        serv_addr=___get_sockaddr_in___(ip,port);
        server_info=___message_whois_next_server___(serv_addr);

        afficherIdentConnexion(server_info);
        return 1;
}


int message_ident(char* ip, uint32_t port){

        struct sockaddr_in serv_addr;
        idConnexion_t server_info;

        serv_addr=___get_sockaddr_in___(ip,port);
        server_info=___message_ident___(serv_addr);

        afficherIdentConnexion(server_info);
        return 1;
}



/*//coupe la socket passé en arg et indique au serveur de couper la communication*/
/*int message_disconnect_from_server(socket_t sockClient){*/

/*        envoyerOrigine(FROM_SERVEUR,sockClient);*/
/*        envoyerTypeMessage(DISCONNECT,sockClient);*/
/*        if(shutdown(sockClient,SHUT_RDWR)==-1){*/
/*                perror("shutdown()");*/
/*                return 0;*/
/*        }*/
/*        return 1;*/
/*}*/




