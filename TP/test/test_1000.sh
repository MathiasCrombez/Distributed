#!/bin/bash

#arguments:
#       $1: nb de clients à lancer



#########mes variables###############""


CLIENT=test_1000_clients
SERVEUR=test_1000_serveur



##execution d'un serveur qui ecoute sur le port 4242
./$SERVEUR $1 & 


for i in `seq 1 $1` ;
do
        ./$CLIENT Client$i localhost 4242 &
#        sleep 1
done


echo "FIN DU TEST"

sleep 30
echo kill serveur
killall --quiet $SERVEUR
#killall --quiet $CLIENT

