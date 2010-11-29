#include "message.h"





uint32_t envoyerMessage(message_t message,socket_t destinataire){

	return send(destinataire, &message, sizeof(message),0);
}


uint32_t recevoirMessage(message_t message, socket_t expediteur){
	
		return recv(expediteur,&message, sizeof(message),0);
}



/** le client demande au serveur dont le nom est nomDuServeur de se déconnecter **/
uint32_t quit(char* nomDuServeur){


	message_t message ;
	message.type = QUIT;
	memcpy((char*)message.requete.quit.nomDuServeurAQuitter,nomDuServeur,MAXCAR);
	envoyerMessage(message,IDSOCKET);

	return 0;

}
