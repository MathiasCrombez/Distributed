.PHONY: clean exec
OBJECTS = serveur.o serveur_impl.o hash.o
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
