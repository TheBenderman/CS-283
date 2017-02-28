CC=gcc
CFLAGS=-g -pthread

client: client.c csapp.h csapp.c
	$(CC) $(CFLAGS) client.c csapp.c -o client.o
	./client.o $(HOST) $(PORT) $(FILE)

server: server.c csapp.h csapp.c
	$(CC) $(CFLAGS) server.c csapp.c -o server.o
	./server.o $(PORT)

clean:
	rm -rf *.o
