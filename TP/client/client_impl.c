#include "client_impl.h"

 
uint32_t connect2server(){
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    // recupération de la socket
    if ((idSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	printf("Erreur socket\n");
	return -1;
    } 
    // Récupération du serveur (adresse)
    if ((server = gethostbyname(SERVEURNAME)) == NULL) {
	printf("Récupération du serveur impossible\n");
	return -1;
    }
    // initialisation de la structure "struct sockaddr_in"
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr, 
	  (char *)server->h_addr,
	  server->h_length);
    // port du serveur ciblé
    serv_addr.sin_port = htons(PORT);
    
    // connection au serveur
    if ( connect( idSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) {
	printf("Echec de la connexion\n");
	return -1;
    }
    printf("client : connecté\n");
    return 0;
}

uint32_t PUT(uint32_t cle, uint32_t valeur){
    if ( write(idSocket,&cle,sizeof(uint32_t)) < 0) {
	printf("PUT : echec de l'envoi de la clé %d\n", cle);
	return -1;
    }
    if ( write(idSocket,&valeur,sizeof(uint32_t)) < 0) {
	printf("PUT : echec de l'envoi de la valeur %d associé à la clé %d \n"
	       , cle, valeur);
	return -1;
    }
    
    return 0;
}

uint32_t GET(uint32_t cle){
    uint32_t valeur = 0;

    if ( write(idSocket,&cle,sizeof(uint32_t)) < 0) {
	printf("GET : echec de l'envoi de la clé %d\n", cle);
	return -1;
    }    
    if( read(idSocket, &valeur, sizeof(uint32_t)) < 0) {
	printf("GET : echec de la lecture de la valeur");
	return -1;
    }
    return valeur;
}

uint32_t REMOVE(uint32_t cle){
    if ( write(idSocket,&cle,sizeof(uint32_t)) < 0) {
	printf("REMOVE : echec de l'envoi de la clé %d\n", cle);
	return -1;
    }    
    return 0;
}


uint32_t disconnect2server(){
    // signal de mort?
    if (shutdown(idSocket, 2) < 0) {
	printf("disconnect2server : echec de la deconnexion\n");
    }
    return 0;
}

