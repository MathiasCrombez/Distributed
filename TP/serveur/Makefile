.PHONY: clean exec
OBJECTS = serveur.o
CFLAGS = -Wall -p -g



serveur : $(OBJECTS)
	gcc $(CFLAGS) -o client $(OBJECTS)
serveur.o : serveur.c
	gcc $(CFLAGS) -c serveur.c


clean :
	rm serveur $(OBJECTS)

exec : 
	./serveur