#!/bin/bash

#arguments:
#       $1: nb de clients à lancer



#########mes variables###############""


CLIENT=test_1000_clients
SERVEUR=test_1000_serveur

if [ $# -lt 1 ]
then 
    echo 'usage : "./test_1000 nombreClients"'
else
    echo 'La communication se fait sur boucle local (127.0.0.1) et le port serveur est le 4242'
##execution d'un serveur qui ecoute sur le port 4242

    ./$SERVEUR & 
    sleep 3
    ./$CLIENT $1 &

    sleep 15
    echo kill serveur
    killall $SERVEUR

fi


