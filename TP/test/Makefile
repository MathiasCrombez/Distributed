all: test_1000 clean

OBJECTS_S = test_1000_serveur.o ../serveur/serveur_impl.o ../serveur/hash.o ../serveur/message.o
OBJECTS_C = test_1000_clients.o ../client/client_impl.o ../client/message.o
CFLAGS = -p -g
IFLAGS_S = -I ../commun/ -I ../serveur
IFLAGS_C = -I ../commun/ -I ../client



<<<<<<< HEAD
test_1000: test_1000_serveur test_1000_clients
=======
####test_1000_serveur########
test_1000_serveur: $(OBJECTS_S) make_s
	gcc $(CFLAGS) -o test_1000_serveur $(OBJECTS_S) -lpthread

test_1000_serveur.o : test_1000_serveur.c
	gcc $(CFLAGS) -c test_1000_serveur.c $(IFLAGS_S)
>>>>>>> feba6583548dcafe0d222e14862a2764bd4ebb35

	$(MAKE) -C ../serveur
<<<<<<< HEAD
test_1000_serveur.o: test_1000_serveur.c
	gcc $(CFLAGS) -c test_1000_serveur.c $(IFLAGS_S)
test_1000_serveur: test_1000_serveur.o
	gcc $(CFLAGS) -o test_1000_serveur $(OBJECTS_S) -lpthread
	
test_1000_clients.o: test_1000_clients.c
	gcc $(CFLAGS) -c test_1000_clients.c $(IFLAGS_C)
test_1000_clients: test_1000_clients.o
=======

#####test_1000_clients#######
test_1000_clients: $(OBJECTS_C)
>>>>>>> feba6583548dcafe0d222e14862a2764bd4ebb35
	gcc $(CFLAGS) -o test_1000_clients $(OBJECTS_C) -lpthread
	


clean:
	rm -f *.o *.out
	rm -f test_1000_clients test_1000_serveur

make_k:
	$(MAKE) -C ../commun

# all: 
# 	$(MAKE) -C ../.

clean:
	rm -f test_1000 test_1000_serveur test_1000_clients test_1000_serveur.o test_1000_clients.o gmon.out
