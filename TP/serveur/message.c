#include"serveur_impl.h"
#include"message.h"

#define DEBUG_MESSAGE_SERVEUR
//#undef  DEBUG_MESSAGE_SERVEUR

#ifdef DEBUG_MESSAGE_SERVEUR
#   define print_debug(chaine, args...)   printf(chaine,##args)
#else
#   define print_debug(chaine, args...)
#endif




//############# PROBLEME LA VAR GLOBAL SERVEUR EST TJRS VIDE!! ###########
int messageConnect2Server(char *to_serveur, uint64_t port)
{

	socket_t idSocket;

	idSocket = connect2server(to_serveur, port);
	if (idSocket == 0) {
		printf("echec de la connexion\n");
		return 0;
	}

	print_debug("connexion au serveur réussie\n");

	printf("SERVEUR.name %s\n", SERVEUR.name);
	printf("mon prochain serveur est:\n");
	afficherIdentConnexion(SERVEUR.nextServeur);

	/* envoi du type de message au serveur */
	envoyerTypeMessage(CONNECT, idSocket);

	recevoirIdent(&SERVEUR.nextServeur.identifiant, idSocket);
	recevoirHash(&(SERVEUR.nextKey), idSocket);
	recevoirChaine(&(SERVEUR.nextServeur.name),idSocket);
	envoyerIdent(SERVEUR.serv_addr, idSocket);
	envoyerHash(SERVEUR.firstKey, idSocket);
	envoyerChaine(SERVEUR.name,idSocket);
	
	printf("... est maintenant mon prochain serveur est:\n");
	afficherIdentConnexion(SERVEUR.nextServeur);
	return 1;
}

int messageParticiperDHT(serveur_t * serveur_ptr)
{

}

