.PHONY: clean exec test_hash
all:serveur test_hash

OBJECTS = serveur.o serveur_impl.o hash.o message.o
CFLAGS = -p -g
IFLAGS = 

#####serveur#####
serveur : $(OBJECTS)
	gcc $(CFLAGS) -o serveur $(OBJECTS) -lpthread
hash.o : hash.c
	gcc $(CFLAGS) -c hash.c $(IFLAGS)
serveur_impl.o : serveur_impl.c
	gcc $(CFLAGS) -c serveur_impl.c $(IFLAGS)
serveur.o : serveur.c
	gcc $(CFLAGS) -c serveur.c $(IFLAGS)
message.o : message.c
	gcc $(CFLAGS) -c message.c $(IFLAGS)
#####test#####	
test_hash:test_hash.o hash.o
	gcc  -g -o test_hash test_hash.o hash.o
test_hash.o:test_hash.c
	gcc -g -c test_hash.c $(IFLAGS)



clean :
	rm -f serveur test_hash $(OBJECTS) test_hash.o gmon.out

exec : 
	./serveur
