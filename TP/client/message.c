#include"message.h"



#define DEBUG_MESSAGE_CLIENT

//TODO ICI DEFINIR LE PROTOCOLE DE COMMUNICATION SEULEMENT ICI!!!


donnee_t  message_get(cle_t K,socket_t from){

	donnee_t D;
	char ack;
	
	envoyerOrigine(FROM_CLIENT,from);
	envoyerTypeMessage(GET,from);
	envoyerCle(K,from);
	recevoirOctet(&ack,from);
	
	if(ack==0){
	#ifdef DEBUG_MESSAGE_CLIENT
		printf("la cle n'existe pas");
	#endif
	}
	else if(ack==1){
		recevoirDonnee(&D,from);
	#ifdef DEBUG_MESSAGE_CLIENT
		afficherDonnee(D);
	#endif
	}
	return D;
}



/*uint32_t put(uint32_t cle, uint32_t valeur)*/
/*{*/
/*	if (write(IDSOCKET, &cle, sizeof(uint32_t)) < 0) {*/
/*		printf("PUT : echec de l'envoi de la clé %d\n", cle);*/
/*		return -1;*/
/*	}*/
/*	if (write(IDSOCKET, &valeur, sizeof(uint32_t)) < 0) {*/
/*		printf*/
/*		    ("PUT : echec de l'envoi de la valeur %d associé à la clé %d \n",*/
/*		     cle, valeur);*/
/*		return -1;*/
/*	}*/

/*	return 0;*/
/*}*/

/*uint32_t get(uint32_t cle)*/
/*{*/
/*	uint32_t valeur = 0;*/

/*	if (write(IDSOCKET, &cle, sizeof(uint32_t)) < 0) {*/
/*		printf("GET : echec de l'envoi de la clé %d\n", cle);*/
/*		return -1;*/
/*	}*/
/*	if (read(IDSOCKET, &valeur, sizeof(uint32_t)) < 0) {*/
/*		printf("GET : echec de la lecture de la valeur");*/
/*		return -1;*/
/*	}*/
/*	return valeur;*/
/*}*/

/*uint32_t removeKey(uint32_t cle)*/
/*{*/
/*	if (write(IDSOCKET, &cle, sizeof(uint32_t)) < 0) {*/
/*		printf("REMOVE : echec de l'envoi de la clé %d\n", cle);*/
/*		return -1;*/
/*	}*/
/*	return 0;*/
/*}*/

/*uint32_t disconnect2server()*/
/*{*/
/*	// signal de mort?*/
/*	if (shutdown(IDSOCKET, 2) < 0) {*/
/*		printf("disconnect2server : echec de la deconnexion\n");*/
/*	}*/
/*	return 0;*/
/*}*/

/*uint32_t quit(char *nomDuServeur)*/
/*{*/

/*	message_t message;*/

/*	message.type = QUIT;*/
/*	memcpy(message.requete.quit.nomDuServeur, nomDuServeur, MAXCAR);*/

/*	envoyerMessage(message, IDSOCKET);*/
/*	recevoirMessage(message, IDSOCKET);*/

/*}*/

