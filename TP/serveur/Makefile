.PHONY: clean exec test
OBJECTS = serveur.o serveur_impl.o hash.o test_hash.o
CFLAGS = -Wall -p -g

IFLAGS = -I ../commun/


serveur : $(OBJECTS)
	gcc $(CFLAGS) -o serveur $(OBJECTS) -lpthread
hash.o : hash.c
	gcc $(CFLAGS) -c hash.c $(IFLAGS)
serveur_impl.o : serveur_impl.c
	gcc $(CFLAGS) -c serveur_impl.c $(IFLAGS)
serveur.o : serveur.c
	gcc $(CFLAGS) -c serveur.c $(IFLAGS)


clean :
	rm -f serveur $(OBJECTS)

exec : 
	./serveur
	
test:test_hash.o hash.o
	gcc  -g -o test_hash test_hash.o hash.o
hash.o: hash.c
	gcc -g -c hash.c -I ../commun/
test_hash.o:test_hash.c
	gcc -g -c test_hash.c -I ../commun/
