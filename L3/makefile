CC=gcc
CFLAGS=-lpthread -g

sync: SyncDirectories.o
	./SyncDirectories.o $(DIR1) $(DIR2)

SyncDirectories.o: SyncDirectories.c csapp.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -rf *.o
