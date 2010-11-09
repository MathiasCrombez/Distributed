#include "client_impl.h"

 
uint32_t connect2server(){
    uint32_t idSocket; 
    uint32_t serverAddr;
    
    if (inet_aton(SERVEURNAME, &serverAddr) < 0) {
	printf("Conversion d'adresse impossible");
	return -1;
    }

    if (idSocket = socket(AF_INET, SOCK_STREAM, 0) < 0) {
	printf("Erreur socket");
	return -1;
    } 
    
    if ( connect( idSocket, (
    return 0;
}

uint32_t PUT(uint32_t cle, uint32_t valeur){
    return 0;
}

uint32_t GET(uint32_t cle){

    return 0;
}

uint32_t REMOVE(uint32_t cle){

    return 0;
}

uint32_t disconnect2server(){

    return 0;
}

