#include "message.h"
#include <sys/socket.h>




uint32_t envoyerMessage(message_t message,socket_t destinataire){

	return send(destinataire, &message, sizeof(message),0);
}


uint32_t recevoirMessage(message_t message, socket_t expediteur){
	
		return recv(expediteur,&message, sizeof(message),0);
}


