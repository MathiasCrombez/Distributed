#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define SERVEURNAME "localhost"
#define PORT 4242

uint32_t idSocket;
// Connecte le cient au serveur et renvoit le port
uint32_t connect2server();

// Envoie un couple clé valeur
uint32_t PUT(uint32_t cle, uint32_t valeur);

// Reçoit la valeur associé à la clé
uint32_t GET(uint32_t cle);

// Supprime une entrée de la DHT
uint32_t REMOVE(uint32_t cle);

uint32_t disconnect2server();

#endif
