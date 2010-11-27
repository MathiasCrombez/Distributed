.PHONY: clean exec
OBJECTS = client.o client_impl.o
CFLAGS = -Wall -p -g



client : $(OBJECTS)
	gcc $(CFLAGS) -o client $(OBJECTS)
client.o : client.c
	gcc $(CFLAGS) -c client.c
client_impl.o : client_impl.c
	gcc $(CFLAGS) -c client_impl.c


clean :
	rm -f client $(OBJECTS)

exec : 
	./client
