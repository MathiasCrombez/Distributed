#!/bin/bash

#arguments:
#       $1: nb de clients à lancer



#########mes variables###############""


CLIENT=test_1000_clients
SERVEUR=test_1000_serveur



##execution d'un serveur qui ecoute sur le port 4242
<<<<<<< HEAD
#./$SERVEUR $1 & 
=======
./$SERVEUR & 
sleep 3
./$CLIENT $1 &
#        sleep 1
>>>>>>> feba6583548dcafe0d222e14862a2764bd4ebb35

sleep 10
echo kill serveur
killall $SERVEUR
#killall --quiet $CLIENT

<<<<<<< HEAD
for i in `seq 1 $1` ;
do

        echo $i
        ./$CLIENT "Client"$i "localhost" "4242" &
        
done


echo "FIN DU TEST"

sleep 30
echo "mort du serveur"
killall --quiet $SERVEUR
killall --quiet $CLIENT
=======
>>>>>>> feba6583548dcafe0d222e14862a2764bd4ebb35
