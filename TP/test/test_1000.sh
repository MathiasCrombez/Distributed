#!/bin/bash

#arguments:
#       $1: nb de clients à lancer



#########mes variables###############""


CLIENT=test_1000_clients
SERVEUR=test_1000_serveur



##execution d'un serveur qui ecoute sur le port 4242
./$SERVEUR & 
sleep 3
./$CLIENT $1 &
#        sleep 1

sleep 10
echo kill serveur
killall $SERVEUR
#killall --quiet $CLIENT

