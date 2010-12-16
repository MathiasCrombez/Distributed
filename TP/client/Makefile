.PHONY: clean exec
OBJECTS = client_impl.o message.o interpreteur.o client.o
#OBJECTS = client_impl.o message.o client.o
CFLAGS =  -p -g

IFLAGS = -I ../commun/


client : $(OBJECTS)
	gcc $(CFLAGS) -o client $(OBJECTS)
interpreteur.o : interpreteur.c 
	gcc $(CFLAGS) -c interpreteur.c $(IFLAGS)
client.o : client.c 
	gcc $(CFLAGS) -c client.c $(IFLAGS)
client_impl.o : client_impl.c
	gcc $(CFLAGS) -c client_impl.c $(IFLAGS)
message.o: message.c
	gcc $(CFLAGS) -c message.c $(IFLAGS)



clean :
	rm -f client $(OBJECTS) gmon.out

exec : 
	./client
