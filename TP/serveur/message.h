#ifndef MESSAGE_H
#define MESSAGE_H

#include"message_commun.h"
#include"liste_donnee.h"

//==============================================================================
//			       MACROS
//==============================================================================


//#define DEBUG_MESSAGE_SERVEUR
#undef  DEBUG_MESSAGE_SERVEUR

#ifdef DEBUG_MESSAGE_SERVEUR
#   define PRINT_DEBUG(chaine, args...)   printf(chaine,##args)
#else
#   define PRINT_DEBUG(chaine, args...)
#endif




//============================================================================
//	                ENVOIS DES TYPES ELEMENTAIRES
//============================================================================

///*
// * Envoi d'une ligne de la table de hachage vers un autre serveur
// * La ligne sera vidé en meme temps
// */
//static int envoyerLigneHashTab(Liste_t L, socket_t to)
//{
//        donnee_t D;
//        while(L!==NULL){
//                D=removeTeteDeListe(&L);
//                assert(D!=NULL);//aucune donné ne devrait etre nul
//                envoyerOctet(1,to);
//                envoyerDonnee(D,to);
//                libererDonnee(D);
//        }
//        //fin d'envoi
//        envoyerOctet(0,to);
//        return 1;
//}

//static int recevoirLigneHashTab(Liste_t* L_ptr, socket_t from)
//{
//        


//}


/*
 * connect au serveur le moins chargé
 */
int message_connect_2_server(char *ip, uint32_t port);

/*
 * se deconnecte du serveur
 */
int message_disconnect_from_server(socket_t sockClient);




#endif
