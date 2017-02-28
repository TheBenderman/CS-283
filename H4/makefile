CC=gcc
CFLAGS=-g

replacestrings: replacestrings.c
	$(CC) $(CFLAGS) replacestrings.c -o replacestrings.o
	./replacestrings.o $(DIRECTORY) $(FIRST) $(SECOND) $(THIRD)

clean:
	rm -rf *.o
