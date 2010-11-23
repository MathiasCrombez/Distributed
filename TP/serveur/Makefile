.PHONY: clean exec
OBJECTS = serveur.o serveur_impl.o hash.o
CFLAGS = -Wall -p -g



serveur : $(OBJECTS)
	gcc $(CFLAGS) -o serveur $(OBJECTS)
hash.o : hash.c
	gcc $(CFLAGS) -c hash.c
serveur_impl.o : serveur_impl.c
	gcc $(CFLAGS) -c serveur_impl.c
serveur.o : serveur.c
	gcc $(CFLAGS) -c serveur.c


clean :
	rm serveur $(OBJECTS)

exec : 
	./serveur