.PHONY: clean exec test_hash
all:serveur test_hash

OBJECTS = serveur.o serveur_impl.o hash.o
CFLAGS =  -p -g

IFLAGS = -I ../commun/

#####serveur#####
serveur : $(OBJECTS)
	gcc $(CFLAGS) -o serveur $(OBJECTS) -lpthread
hash.o : hash.c ../commun/commun.h
	gcc $(CFLAGS) -c hash.c $(IFLAGS)
serveur_impl.o : serveur_impl.c
	gcc $(CFLAGS) -c serveur_impl.c $(IFLAGS)
serveur.o : serveur.c
	gcc $(CFLAGS) -c serveur.c $(IFLAGS)

#####test#####	
test_hash:test_hash.o hash.o
	gcc  -g -o test_hash test_hash.o hash.o
test_hash.o:test_hash.c ../commun/commun.h
	gcc -g -c test_hash.c -I ../commun



clean :
	rm -f serveur test_hash $(OBJECTS) test_hash.o gmon.out

exec : 
	./serveur
